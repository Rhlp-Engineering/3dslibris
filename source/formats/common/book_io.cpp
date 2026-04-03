/*
    3dslibris - book_io.cpp
    New module for the 3DS port by Rigle.

    Summary:
    - Input/output and parser dispatch for non-EPUB formats.
    - UTF-8 normalization and encoding repair utilities.
    - TXT/RTF/ODT loading, extraction, and chapter/index helper generation.
*/

#include "book/book.h"
#include "book/book_parse_deps.h"

#include "shared/status_reporter.h"
#include "book/book_xml.h"
#include "formats/common/book_error.h"
#include "debug_log.h"
#include "formats/common/plain_parser.h"
#include "formats/common/plain_text_perf_utils.h"
#include "formats/mobi/mobi.h"
#include "formats/mobi/mobi_deferred_runtime.h"
#include "formats/mobi/mobi_markup_extract.h"
#include "formats/mobi/mobi_parser.h"
#include "formats/mobi/mobi_toc_finalize.h"
#include "formats/mobi/mobi_toc_prepare.h"
#include "formats/mobi/mobi_text_decode.h"
#include "formats/pdf/pdf.h"
#include "formats/cbz/cbz.h"
#include "formats/common/page_cache_utils.h"
#include "formats/common/xml_parse_utils.h"
#include "parse.h"
#include "formats/odt/odt_loader.h"
#include "shared/parser_limits.h"
#include "string_utils.h"
#include "shared/utf8_utils.h"
#include <algorithm>
#include <ctype.h>
#include <set>
#include <stdio.h>
#include <string.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <utility>
#include <vector>

namespace {

static const size_t kPlainTextMaxBytes = 12 * 1024 * 1024;

typedef BookParseDeps BookIoDeps;
using plain_text_perf_utils::CapturePlainTextPerfBaseline;
using plain_text_perf_utils::FillPlainTextStreamPerf;
using plain_text_perf_utils::LogPlainTextStreamPerf;
using plain_text_perf_utils::PlainTextPerfBaseline;
using plain_text_perf_utils::PlainTextStreamPerf;
using mobi_toc_finalize::MobiHeadingHint;

static BookIoDeps BuildBookIoDeps(Book *book) { return BuildBookParseDeps(book); }

static void InitParsedataWithBookIoDeps(parsedata_t *parsedata, Book *book,
                                        const BookIoDeps &deps) {
  if (!parsedata)
    return;
  parse_init(parsedata);
  parsedata->reporter = deps.reporter;
  parsedata->ts = deps.ts;
  parsedata->prefs = deps.prefs;
  parsedata->book = book;
}

static std::string DecodeMobiBytesToUtf8(const std::string &in, u32 encoding,
                                         bool *used_utf8_guess,
                                         bool *used_legacy_guess) {
  return mobi_text_decode::DecodeBytesToUtf8(in, encoding, used_utf8_guess,
                                             used_legacy_guess);
}

static size_t PruneMobiFrontMatterTocCluster(Book *book, IStatusReporter *reporter) {
  if (!book)
    return 0;
  const std::vector<ChapterEntry> &chapters = book->GetChapters();
  if (chapters.size() < 20)
    return 0;

  const u16 page_count = book->GetPageCount();
  if (page_count < 220)
    return 0;

  // In large MOBI books, printed "Table of Contents" pages can be mapped as
  // dozens of pseudo-chapters concentrated near the beginning.
  u16 front_page_limit = page_count / 18;
  if (front_page_limit < 28)
    front_page_limit = 28;
  if (front_page_limit > 96)
    front_page_limit = 96;

  size_t prefix_count = 0;
  std::set<u16> prefix_unique_pages;
  for (size_t i = 0; i < chapters.size(); i++) {
    if (chapters[i].page > front_page_limit)
      break;
    prefix_count++;
    prefix_unique_pages.insert(chapters[i].page);
  }

  if (prefix_count < 12)
    return 0;
  if (prefix_count + 8 >= chapters.size())
    return 0; // Avoid pruning when almost everything is in front matter.

  const size_t uniq = prefix_unique_pages.size();
  const bool highly_dense = (uniq * 100 < prefix_count * 55);
  const bool large_prefix = (prefix_count * 100 >= chapters.size() * 35);
  if (!highly_dense || !large_prefix)
    return 0;

  std::vector<ChapterEntry> kept;
  kept.reserve(chapters.size() - prefix_count);
  for (size_t i = prefix_count; i < chapters.size(); i++)
    kept.push_back(chapters[i]);

  if (kept.size() < 6)
    return 0;

  book->ClearChapters();
  for (size_t i = 0; i < kept.size(); i++)
    book->AddChapter(kept[i].page, kept[i].title, kept[i].level);

  if (reporter) {
    char msg[224];
    snprintf(
        msg, sizeof(msg),
        "MOBI: TOC front-matter pruned removed=%u remain=%u front_limit<=%u",
        (unsigned)prefix_count, (unsigned)kept.size(),
        (unsigned)front_page_limit);
    DBG_LOG(reporter, msg);
  }
  return prefix_count;
}

static bool LooksLikeStructuredMobiChapterTitle(const std::string &title) {
  std::string compact =
      plain_parser::CollapseAsciiWhitespace(plain_parser::TrimAsciiWhitespace(title));
  if (compact.empty())
    return false;
  std::string folded = plain_parser::FoldLatinForMatch(compact);
  bool strong_signal = false;
  if (plain_parser::LooksLikePlainChapterHeading(compact, &strong_signal))
    return true;

  return false;
}

static bool IsMobiHeuristicChapterSetNoisy(Book *book,
                                           mobi_toc_finalize::MobiChapterQualityStats *stats_out) {
  if (!book)
    return false;
  const std::vector<ChapterEntry> &chapters = book->GetChapters();
  if (chapters.size() < 8)
    return false;

  const u16 page_count = book->GetPageCount();
  if (page_count == 0)
    return false;

  u16 early_window = page_count / 12; // ~first 8%
  if (early_window < 12)
    early_window = 12;
  if (early_window > 96)
    early_window = 96;

  size_t early_hits = 0;
  size_t tiny_titles = 0;
  size_t noisy_titles = 0;
  size_t structured_titles = 0;
  std::set<u16> unique_pages;
  for (size_t i = 0; i < chapters.size(); i++) {
    const ChapterEntry &c = chapters[i];
    unique_pages.insert(c.page);
    if (c.page <= early_window)
      early_hits++;
    std::string clean = plain_parser::CollapseAsciiWhitespace(
        plain_parser::TrimAsciiWhitespace(c.title));
    if (clean.size() < 4)
      tiny_titles++;
    if (plain_parser::IsMostlyDigitsOrPunctuation(clean))
      noisy_titles++;
    if (LooksLikeStructuredMobiChapterTitle(clean))
      structured_titles++;
  }

  if (stats_out) {
    stats_out->chapters = chapters.size();
    stats_out->unique_pages = unique_pages.size();
    stats_out->early_hits = early_hits;
    stats_out->tiny_titles = tiny_titles;
    stats_out->noisy_titles = noisy_titles;
    stats_out->structured_titles = structured_titles;
    stats_out->early_window = early_window;
  }

  const size_t n = chapters.size();
  const bool mostly_early = (early_hits * 100 >= n * 65);
  const bool low_spread = (unique_pages.size() * 100 < n * 35);
  const bool mostly_noisy_titles =
      ((tiny_titles + noisy_titles) * 100 >= n * 55);
  const bool too_unstructured = (structured_titles * 100 < n * 35);
  return mostly_early || low_spread || mostly_noisy_titles || too_unstructured;
}


static std::string DecodeMobiStructuredTocBytes(const std::string &in,
                                                u32 encoding) {
  return DecodeMobiBytesToUtf8(in, encoding, NULL, NULL);
}

static std::string NormalizeMobiStructuredTocTitle(const std::string &in) {
  return plain_parser::CollapseAsciiWhitespace(
      plain_parser::TrimAsciiWhitespace(in));
}

static bool RejectMobiStructuredTocTitle(const std::string &in) {
  return plain_parser::IsMostlyDigitsOrPunctuation(in);
}

static std::string NormalizeHeadingNeedle(const std::string &s) {
  std::string trimmed = plain_parser::TrimAsciiWhitespace(s);
  for (size_t i = 0; i < trimmed.size(); i++) {
    if (trimmed[i] == '|')
      trimmed[i] = ' ';
  }
  return plain_parser::CollapseAsciiWhitespace(
      plain_parser::FoldLatinForMatch(trimmed));
}

static bool PageHasHeadingNeedle(const std::vector<std::string> &lines,
                                 const std::string &needle) {
  if (needle.empty())
    return false;
  // Search all lines on the page.  Pages are short (3DS screen ≈ 30-40 lines)
  // and decorative whitespace (pagebreaks, <br>) generates many blank entries
  // that previously exhausted the old cap of 18 before reaching real content.
  const size_t n = lines.size();
  for (size_t i = 0; i < n; i++) {
    std::string norm = NormalizeHeadingNeedle(lines[i]);
    if (norm.empty())
      continue;
    if (norm == needle)
      return true;
    if (norm.size() > needle.size() && norm.find(needle) != std::string::npos)
      return true;
  }
  // Multi-line headings: try concatenating consecutive non-empty lines.
  for (size_t i = 0; i + 1 < n; i++) {
    std::string a = NormalizeHeadingNeedle(lines[i]);
    if (a.empty())
      continue;
    std::string concat = a;
    for (size_t j = i + 1; j < n && concat.size() < needle.size() + 32; j++) {
      std::string b = NormalizeHeadingNeedle(lines[j]);
      if (b.empty())
        continue;
      concat += " " + b;
      if (concat == needle)
        return true;
      if (concat.size() > needle.size() &&
          concat.find(needle) != std::string::npos)
        return true;
    }
  }
  return false;
}

static mobi_toc_finalize::FinalizeCallbacks MakeMobiTocFinalizeCallbacks() {
  mobi_toc_finalize::FinalizeCallbacks callbacks;
  callbacks.normalize_heading_needle = NormalizeHeadingNeedle;
  callbacks.page_has_heading_needle = PageHasHeadingNeedle;
  callbacks.add_chapter_at_page_if_unique = plain_parser::AddChapterAtPageIfUnique;
  callbacks.prune_front_matter_toc_cluster = PruneMobiFrontMatterTocCluster;
  callbacks.is_heuristic_chapter_set_noisy = IsMobiHeuristicChapterSetNoisy;
  return callbacks;
}

static size_t MobiInlineCountWords(const std::string &text) {
  return (size_t)plain_parser::CountAsciiWords(text);
}

static bool MobiInlineMostlyDigitsOrPunct(const std::string &text) {
  return plain_parser::IsMostlyDigitsOrPunctuation(text);
}

static mobi_toc_prepare::StructuredCallbacks
MakeMobiStructuredTocCallbacks() {
  mobi_toc_prepare::StructuredCallbacks callbacks;
  callbacks.decode_bytes_to_utf8 = DecodeMobiStructuredTocBytes;
  callbacks.normalize_title = NormalizeMobiStructuredTocTitle;
  callbacks.reject_title = RejectMobiStructuredTocTitle;
  return callbacks;
}

static mobi_toc_prepare::InlineTitleCallbacks MakeMobiInlineTitleCallbacks() {
  mobi_toc_prepare::InlineTitleCallbacks callbacks;
  callbacks.looks_like_structured_title = LooksLikeStructuredMobiChapterTitle;
  callbacks.fold_latin_for_match = plain_parser::FoldLatinForMatch;
  callbacks.count_ascii_words = MobiInlineCountWords;
  callbacks.is_mostly_digits_or_punctuation = MobiInlineMostlyDigitsOrPunct;
  return callbacks;
}

static mobi_markup_extract::ExtractCallbacks MakeMobiMarkupExtractCallbacks() {
  mobi_markup_extract::ExtractCallbacks callbacks;
  callbacks.trim_ascii_whitespace = plain_parser::TrimAsciiWhitespace;
  callbacks.collapse_ascii_whitespace = plain_parser::CollapseAsciiWhitespace;
  callbacks.fold_latin_for_match = plain_parser::FoldLatinForMatch;
  callbacks.looks_like_structured_chapter_title = LooksLikeStructuredMobiChapterTitle;
  callbacks.add_chapter_at_page_if_unique = plain_parser::AddChapterAtPageIfUnique;
  return callbacks;
}

static std::string ExtractMobiMarkupToText(
    Book *book, const BookIoDeps &deps, const std::string &in,
    std::vector<MobiHeadingHint> *heading_hints,
    std::vector<std::pair<u32, u32>> *html_to_text_map) {
  return mobi_markup_extract::ExtractToText(
      book, deps, in, heading_hints, html_to_text_map,
      MakeMobiMarkupExtractCallbacks());
}

static mobi_parser::Hooks MakeMobiParserHooks() {
  mobi_parser::Hooks hooks;
  hooks.extract_markup_to_text = ExtractMobiMarkupToText;
  hooks.make_structured_toc_callbacks = MakeMobiStructuredTocCallbacks;
  hooks.make_inline_title_callbacks = MakeMobiInlineTitleCallbacks;
  hooks.make_finalize_callbacks = MakeMobiTocFinalizeCallbacks;
  hooks.make_plain_continue_callbacks = plain_parser::MakeContinueCallbacks;
  return hooks;
}

} // namespace

u8 Book::Parse(bool fulltext) {
  //! Parse full text (true) or titles only (false).
  //! Expat callback handlers do the heavy work.
  u8 rc = 0;

  char path[MAXPATHLEN];
  snprintf(path, sizeof(path), "%s/%s", GetFolderName(), GetFileName());

  // Lightweight non-XML formats.
  if (fulltext && HasExtCI(GetFileName(), ".txt"))
    return plain_parser::ParseTxtFile(this, path);
  if (fulltext && HasExtCI(GetFileName(), ".rtf"))
    return plain_parser::ParseRtfFile(this, path);
  if (fulltext && HasExtCI(GetFileName(), ".odt"))
    return odt_loader::ParseOdtFile(this, path);
  if (fulltext && HasExtCI(GetFileName(), ".mobi"))
    return mobi_parser::ParseFile(this, path, MakeMobiParserHooks());
  if (fulltext && (HasExtCI(GetFileName(), ".pdf") ||
                   HasExtCI(GetFileName(), ".xps") ||
                   HasExtCI(GetFileName(), ".oxps")))
    return ParsePdfFile(this, path);
  if (fulltext && HasExtCI(GetFileName(), ".cbz"))
    return ParseCbzFile(this, path);

  FILE *fp = fopen(path, "r");
  if (!fp) {
    rc = 255;
    return (rc);
  }

  const BookIoDeps deps = BuildBookIoDeps(this);
  parsedata_t parsedata;
  InitParsedataWithBookIoDeps(&parsedata, this, deps);
  parsedata.fb2_mode = fulltext && HasExtCI(GetFileName(), ".fb2");
#ifdef DSLIBRIS_DEBUG
  const u64 xml_parse_begin = osGetTime();
  const u16 xml_pages_before = GetPageCount();
#endif
  PlainTextPerfBaseline xml_perf_baseline;
  CapturePlainTextPerfBaseline(parsedata, &xml_perf_baseline);

  xml_parse_utils::XmlParserOptions options;
  options.user_data = &parsedata;
  options.default_handler = xml::book::fallback;
  options.processing_instruction = xml::book::instruction;
  options.start_element = xml::book::start;
  options.end_element = xml::book::end;
  options.character_data = xml::book::chardata;
  if (!fulltext) {
    options.start_element = xml::book::metadata::start;
    options.end_element = xml::book::metadata::end;
    options.character_data = xml::book::metadata::chardata;
  }
  xml_parse_utils::XmlParseResult parse_result =
      xml_parse_utils::ParseXmlFileStream(
          fp, options, parser_limits::kXmlStreamBufferSize,
          [](void *user_data) {
            parsedata_t *parsedata = static_cast<parsedata_t *>(user_data);
            return parsedata && parsedata->status;
          });
  fclose(fp);
  if (!parse_result.ok) {
    if (deps.reporter)
      deps.reporter->PrintStatus(
          xml_parse_utils::FormatXmlParseError(parse_result).c_str());
    rc = 254;
  }

  if (rc == 0 && fulltext && parsedata.fb2_mode) {
    bool has_structured_toc = !chapters.empty();
    if (!has_structured_toc)
      plain_parser::BuildFb2FallbackChapters(this);
    if (!chapters.empty())
      plain_parser::SetNonEpubTocConfidence(this, has_structured_toc);
    else
      ClearTocConfidence();
  }

#ifdef DSLIBRIS_DEBUG
  if (deps.reporter && fulltext) {
    PlainTextStreamPerf perf;
    FillPlainTextStreamPerf(parsedata, xml_perf_baseline,
                            osGetTime() - xml_parse_begin, 0,
                            GetPageCount() - xml_pages_before, &perf);
    LogPlainTextStreamPerf(
        deps.reporter, parsedata.fb2_mode ? "FB2 layout" : "XML layout", perf,
        rc == 0);
  }
#endif

  return (rc);
}

bool Book::HasDeferredMobiParse() const {
  return mobi_deferred_runtime::Has(this);
}

bool Book::ContinueDeferredMobiParse(u32 budget_ms, u16 page_budget) {
  return mobi_parser::ContinueDeferredParse(this, budget_ms, page_budget,
                                            MakeMobiParserHooks());
}

void Book::CancelDeferredMobiParse() { mobi_deferred_runtime::Erase(this); }

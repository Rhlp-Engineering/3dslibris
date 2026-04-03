/*
    3dslibris - mobi_toc_resolver.cpp

    MOBI TOC resolver helpers (inline filepos fallback + deferred loading).
*/

#include "formats/mobi/mobi_toc_resolver.h"

#include <algorithm>
#include <ctype.h>
#include <map>
#include <stdio.h>
#include <string.h>

#include "debug_log.h"
#include "formats/common/file_read_utils.h"
#include "formats/mobi/mobi_record_decode.h"
#include "formats/mobi/mobi_record_scan.h"
#include "shared/status_reporter.h"

namespace {

static const u32 kMobiNullIndex = mobi_structured_toc_parser::kMobiNullIndex;
typedef mobi_structured_toc_parser::MobiStructuredTocEntry MobiStructuredTocEntry;
static const size_t kMobiMaxBytes = 64 * 1024 * 1024;

bool DecodeHtmlEntity(const std::string &entity, std::string *out) {
  if (!out)
    return false;
  if (entity.empty())
    return false;
  if (entity[0] == '#') {
    unsigned long parsed = 0;
    if (entity.size() >= 2 && (entity[1] == 'x' || entity[1] == 'X')) {
      if (sscanf(entity.c_str() + 2, "%lx", &parsed) != 1)
        return false;
    } else {
      if (sscanf(entity.c_str() + 1, "%lu", &parsed) != 1)
        return false;
    }
    u32 cp = (u32)parsed;
    if (cp == 0)
      return false;
    if (cp < 0x80u) {
      out->push_back((char)cp);
    } else if (cp < 0x800u) {
      out->push_back((char)(0xC0u | (cp >> 6)));
      out->push_back((char)(0x80u | (cp & 0x3Fu)));
    } else if (cp < 0x10000u) {
      out->push_back((char)(0xE0u | (cp >> 12)));
      out->push_back((char)(0x80u | ((cp >> 6) & 0x3Fu)));
      out->push_back((char)(0x80u | (cp & 0x3Fu)));
    } else {
      out->push_back((char)(0xF0u | (cp >> 18)));
      out->push_back((char)(0x80u | ((cp >> 12) & 0x3Fu)));
      out->push_back((char)(0x80u | ((cp >> 6) & 0x3Fu)));
      out->push_back((char)(0x80u | (cp & 0x3Fu)));
    }
    return true;
  }

  if (entity == "amp")
    out->push_back('&');
  else if (entity == "lt")
    out->push_back('<');
  else if (entity == "gt")
    out->push_back('>');
  else if (entity == "quot")
    out->push_back('"');
  else if (entity == "apos")
    out->push_back('\'');
  else if (entity == "nbsp")
    out->push_back(' ');
  else if (entity == "mdash")
    out->append("\xE2\x80\x94");
  else if (entity == "ndash")
    out->append("\xE2\x80\x93");
  else if (entity == "hellip")
    out->append("\xE2\x80\xA6");
  else
    return false;
  return true;
}

size_t FindAsciiNoCase(const std::string &haystack, const char *needle,
                       size_t start, size_t limit) {
  if (!needle || !*needle)
    return std::string::npos;
  if (start >= haystack.size())
    return std::string::npos;
  if (limit > haystack.size())
    limit = haystack.size();
  if (start >= limit)
    return std::string::npos;

  const size_t nlen = strlen(needle);
  if (nlen == 0 || start + nlen > limit)
    return std::string::npos;

  for (size_t i = start; i + nlen <= limit; i++) {
    size_t j = 0;
    for (; j < nlen; j++) {
      unsigned char a = (unsigned char)haystack[i + j];
      unsigned char b = (unsigned char)needle[j];
      if (tolower(a) != tolower(b))
        break;
    }
    if (j == nlen)
      return i;
  }
  return std::string::npos;
}

bool ParseMobiAnchorFilepos(const std::string &tag, u32 *out_pos) {
  if (!out_pos)
    return false;
  *out_pos = kMobiNullIndex;
  if (tag.empty())
    return false;

  size_t p = FindAsciiNoCase(tag, "filepos", 0, tag.size());
  if (p == std::string::npos)
    return false;
  p += 7;
  while (p < tag.size() && isspace((unsigned char)tag[p]))
    p++;
  if (p >= tag.size() || tag[p] != '=')
    return false;
  p++;
  while (p < tag.size() && isspace((unsigned char)tag[p]))
    p++;
  if (p >= tag.size())
    return false;

  char quote = 0;
  if (tag[p] == '\'' || tag[p] == '"')
    quote = tag[p++];
  if (p >= tag.size())
    return false;

  size_t d0 = p;
  while (p < tag.size() && isdigit((unsigned char)tag[p]))
    p++;
  if (p <= d0)
    return false;
  if (quote != 0 && (p >= tag.size() || tag[p] != quote))
    return false;

  unsigned long long parsed = 0;
  if (sscanf(tag.substr(d0, p - d0).c_str(), "%llu", &parsed) != 1)
    return false;
  if (parsed == 0ull || parsed > 0xFFFFFFFFull)
    return false;
  *out_pos = (u32)parsed;
  return true;
}

std::string TrimAsciiWhitespaceLocal(const std::string &in) {
  size_t start = 0;
  while (start < in.size() && isspace((unsigned char)in[start]))
    start++;
  size_t end = in.size();
  while (end > start && isspace((unsigned char)in[end - 1]))
    end--;
  return in.substr(start, end - start);
}

std::string CollapseAsciiWhitespaceLocal(const std::string &in) {
  std::string out;
  out.reserve(in.size());
  bool last_space = false;
  for (size_t i = 0; i < in.size(); i++) {
    unsigned char c = (unsigned char)in[i];
    if (isspace(c)) {
      if (!last_space && !out.empty()) {
        out.push_back(' ');
        last_space = true;
      }
      continue;
    }
    out.push_back((char)c);
    last_space = false;
  }
  return out;
}

std::string NormalizeInlineTitle(const std::string &in) {
  return CollapseAsciiWhitespaceLocal(TrimAsciiWhitespaceLocal(in));
}

std::string DecodeMobiAnchorInnerText(const std::string &in) {
  std::string out;
  out.reserve(in.size());
  bool in_tag = false;
  bool pending_space = false;

  for (size_t i = 0; i < in.size();) {
    unsigned char c = (unsigned char)in[i];
    if (c == '<') {
      in_tag = true;
      pending_space = true;
      i++;
      continue;
    }
    if (in_tag) {
      if (c == '>')
        in_tag = false;
      i++;
      continue;
    }

    if (c == '&') {
      size_t semi = in.find(';', i + 1);
      if (semi != std::string::npos && semi - i <= 12) {
        std::string entity = in.substr(i + 1, semi - i - 1);
        std::string decoded;
        if (DecodeHtmlEntity(entity, &decoded)) {
          if (pending_space && !out.empty() && out.back() != ' ')
            out.push_back(' ');
          out += decoded;
          pending_space = false;
          i = semi + 1;
          continue;
        }
      }
    }

    if (isspace(c) || c < 0x20) {
      pending_space = true;
      i++;
      continue;
    }

    if (pending_space && !out.empty() && out.back() != ' ')
      out.push_back(' ');
    pending_space = false;

    if (c < 0x80) {
      out.push_back((char)c);
      i++;
      continue;
    }

    int step = 1;
    if ((c & 0xE0) == 0xC0)
      step = 2;
    else if ((c & 0xF0) == 0xE0)
      step = 3;
    else if ((c & 0xF8) == 0xF0)
      step = 4;
    if (i + (size_t)step > in.size())
      step = 1;
    out.append(in, i, (size_t)step);
    i += (size_t)step;
  }

  return NormalizeInlineTitle(out);
}

bool IsMobiLikelyTocTitle(const std::string &title,
                          const mobi_toc_resolver::InlineTitleCallbacks &callbacks) {
  if (!callbacks.looks_like_structured_title || !callbacks.fold_latin_for_match ||
      !callbacks.count_ascii_words || !callbacks.is_mostly_digits_or_punctuation) {
    return false;
  }

  std::string clean = NormalizeInlineTitle(title);
  if (clean.size() < 3 || clean.size() > 180)
    return false;
  if (callbacks.is_mostly_digits_or_punctuation(clean))
    return false;

  std::string folded = callbacks.fold_latin_for_match(clean);
  auto contains_ci = [&](const char *needle) -> bool {
    return (needle && *needle && folded.find(needle) != std::string::npos);
  };
  if (folded == "contents" || folded == "table of contents" ||
      folded == "indice" || folded == "indice de contenido" ||
      folded == "contenido")
    return false;
  if (folded == "index" || folded == "this page" || folded == "cover" ||
      folded == "title page" || folded == "copyright")
    return false;
  if (contains_ci("this page") || contains_ci("page") || contains_ci("back") ||
      contains_ci("next") || contains_ci("previous") || contains_ci("menu") ||
      contains_ci("search") || contains_ci("table of contents") ||
      contains_ci("contents"))
    return false;
  if (contains_ci("ep_prh_") || contains_ci("kindle:") ||
      contains_ci("navpoint"))
    return false;

  if (callbacks.count_ascii_words(clean) > 20)
    return false;

  return true;
}

int ScoreMobiTocTitle(const std::string &title,
                      const mobi_toc_resolver::InlineTitleCallbacks &callbacks) {
  std::string clean = NormalizeInlineTitle(title);
  if (clean.empty())
    return -1000;

  int score = 0;
  const int words = (int)callbacks.count_ascii_words(clean);
  if (words >= 2 && words <= 12)
    score += 20;
  if (words >= 13 && words <= 18)
    score += 8;
  if (words <= 1 || words > 20)
    score -= 30;

  if (callbacks.looks_like_structured_title(clean))
    score += 40;

  std::string folded = callbacks.fold_latin_for_match(clean);
  if (folded == "this page" || folded == "index" || folded == "contents" ||
      folded == "table of contents")
    score -= 80;
  if (folded.find("this page") != std::string::npos ||
      folded.find(" table of contents") != std::string::npos)
    score -= 60;

  if ((int)clean.size() >= 8 && (int)clean.size() <= 80)
    score += 10;
  else if ((int)clean.size() < 5)
    score -= 25;

  return score;
}

} // namespace

namespace mobi_toc_resolver {

bool ParseInlineFileposToc(const std::string &markup_utf8, u32 text_len,
                           const InlineTitleCallbacks &callbacks,
                           std::vector<MobiStructuredTocEntry> *out,
                           IStatusReporter *reporter) {
  if (!out)
    return false;
  out->clear();
  if (markup_utf8.empty())
    return false;

  if (!callbacks.looks_like_structured_title || !callbacks.fold_latin_for_match ||
      !callbacks.count_ascii_words || !callbacks.is_mostly_digits_or_punctuation) {
    return false;
  }

  size_t full_scan_limit = markup_utf8.size();
  if (text_len > 0) {
    size_t hint = (size_t)text_len / 3;
    if (hint < 256 * 1024)
      hint = 256 * 1024;
    if (hint > 1024 * 1024)
      hint = 1024 * 1024;
    if (hint < full_scan_limit)
      full_scan_limit = hint;
  }
  if (full_scan_limit > markup_utf8.size())
    full_scan_limit = markup_utf8.size();

  size_t first_pass_limit = full_scan_limit;
  if (first_pass_limit > 512 * 1024)
    first_pass_limit = 384 * 1024;

  std::vector<MobiStructuredTocEntry> raw_entries;
  raw_entries.reserve(256);

  auto scan_for_entries = [&](size_t scan_limit) {
    size_t pos = 0;
    while (pos < scan_limit) {
      size_t a0 = markup_utf8.find("<a", pos);
      if (a0 == std::string::npos || a0 >= scan_limit)
        a0 = FindAsciiNoCase(markup_utf8, "<a", pos, scan_limit);
      if (a0 == std::string::npos)
        break;

      size_t tag_end = markup_utf8.find('>', a0 + 2);
      if (tag_end == std::string::npos || tag_end >= scan_limit)
        break;
      if (tag_end <= a0 + 2 || (tag_end - (a0 + 2)) > 640) {
        pos = tag_end + 1;
        continue;
      }

      std::string tag = markup_utf8.substr(a0 + 2, tag_end - (a0 + 2));
      u32 filepos = kMobiNullIndex;
      if (!ParseMobiAnchorFilepos(tag, &filepos)) {
        pos = tag_end + 1;
        continue;
      }

      if (filepos == 0 || filepos == kMobiNullIndex) {
        pos = tag_end + 1;
        continue;
      }
      if (text_len > 0) {
        u32 allowed_max = text_len + (text_len / 8) + 1024;
        if (filepos > allowed_max) {
          pos = tag_end + 1;
          continue;
        }
      }

      const size_t close_probe_limit =
          std::min(scan_limit, tag_end + (size_t)2048);
      size_t close = markup_utf8.find("</a", tag_end + 1);
      if (close == std::string::npos || close >= close_probe_limit)
        close =
            FindAsciiNoCase(markup_utf8, "</a", tag_end + 1, close_probe_limit);
      if (close == std::string::npos || close <= tag_end) {
        pos = tag_end + 1;
        continue;
      }

      size_t inner_len = close - (tag_end + 1);
      if (inner_len > 512)
        inner_len = 512;
      std::string inner = markup_utf8.substr(tag_end + 1, inner_len);
      std::string title = DecodeMobiAnchorInnerText(inner);
      if (!IsMobiLikelyTocTitle(title, callbacks)) {
        pos = close + 4;
        continue;
      }

      MobiStructuredTocEntry e;
      e.title = title;
      e.pos = filepos;
      e.level = 0;
      raw_entries.push_back(e);

      pos = close + 4;
      if (raw_entries.size() >= 2048)
        break;
    }
  };

  scan_for_entries(first_pass_limit);
  size_t used_scan_limit = first_pass_limit;
  if (raw_entries.size() < 8 && first_pass_limit < full_scan_limit) {
    raw_entries.clear();
    scan_for_entries(full_scan_limit);
    used_scan_limit = full_scan_limit;
  }

  if (raw_entries.size() < 2)
    return false;

  std::map<u32, MobiStructuredTocEntry> by_pos;
  for (size_t i = 0; i < raw_entries.size(); i++) {
    const MobiStructuredTocEntry &e = raw_entries[i];
    std::map<u32, MobiStructuredTocEntry>::iterator it = by_pos.find(e.pos);
    if (it == by_pos.end()) {
      by_pos[e.pos] = e;
      continue;
    }
    int cur_score = ScoreMobiTocTitle(it->second.title, callbacks);
    int new_score = ScoreMobiTocTitle(e.title, callbacks);
    if (new_score > cur_score ||
        (new_score == cur_score && e.title.size() > it->second.title.size())) {
      it->second = e;
    }
  }

  size_t structured_like = 0;
  size_t low_quality = 0;
  for (std::map<u32, MobiStructuredTocEntry>::const_iterator kv = by_pos.begin();
       kv != by_pos.end(); ++kv) {
    const MobiStructuredTocEntry &e = kv->second;
    if (callbacks.looks_like_structured_title(e.title))
      structured_like++;
    if (ScoreMobiTocTitle(e.title, callbacks) < 0)
      low_quality++;
    if (!out->empty() && out->back().pos == e.pos &&
        out->back().title == e.title)
      continue;
    out->push_back(e);
  }

  if (out->size() < 2) {
    out->clear();
    return false;
  }

  size_t min_entries = (text_len > 1000000u) ? 8u : 4u;
  bool weak_size = out->size() < min_entries;
  bool weak_structure =
      (out->size() >= 4 && structured_like * 100 < out->size() * 30);
  bool weak_quality =
      (out->size() >= 4 && low_quality * 100 > out->size() * 55);
  if (weak_size || weak_structure || weak_quality) {
    if (reporter) {
      char msg[220];
      snprintf(
          msg, sizeof(msg),
          "MOBI: filepos TOC rejected kept=%u structured=%u lowq=%u min=%u",
          (unsigned)out->size(), (unsigned)structured_like,
          (unsigned)low_quality, (unsigned)min_entries);
      DBG_LOG(reporter, msg);
    }
    out->clear();
    return false;
  }

  if (reporter) {
    char msg[224];
    snprintf(msg, sizeof(msg),
             "MOBI: filepos TOC entries raw=%u kept=%u structured=%u scan=%uKB",
             (unsigned)raw_entries.size(), (unsigned)out->size(),
             (unsigned)structured_like, (unsigned)(used_scan_limit / 1024));
    DBG_LOG(reporter, msg);
  }
  return true;
}

bool PrepareStructuredToc(const std::string &raw, const std::vector<u32> &offsets,
                          u32 ncx_index, u32 encoding,
                          const std::string *markup_utf8, u32 text_len,
                          const PrepareCallbacks &callbacks,
                          std::vector<MobiStructuredTocEntry> *out,
                          bool *structured_from_filepos,
                          IStatusReporter *reporter) {
  if (!out)
    return false;
  out->clear();
  if (structured_from_filepos)
    *structured_from_filepos = false;

  if (!callbacks.structured.decode_bytes_to_utf8 ||
      !callbacks.structured.normalize_title ||
      !callbacks.structured.reject_title) {
    return false;
  }

  if (mobi_structured_toc_parser::ParseStructuredToc(
          raw, offsets, ncx_index, encoding, callbacks.structured, out,
          reporter)) {
    return true;
  }

  if (markup_utf8 &&
      ParseInlineFileposToc(*markup_utf8, text_len, callbacks.inline_title, out,
                            reporter)) {
    if (structured_from_filepos)
      *structured_from_filepos = true;
    return true;
  }

  return false;
}

bool LoadDeferredStructuredToc(
    const std::vector<MobiStructuredTocEntry> *cached_toc, bool have_cached_toc,
    const std::string &markup_utf8, u32 text_len_for_pos,
    const std::string &source_path, const PrepareCallbacks &callbacks,
    std::vector<MobiStructuredTocEntry> *out, bool *structured_from_filepos,
    IStatusReporter *reporter) {
  if (!out)
    return false;
  if (structured_from_filepos)
    *structured_from_filepos = false;

  if (cached_toc && have_cached_toc) {
    *out = *cached_toc;
    if (reporter) {
      DBG_LOGF(reporter, "MOBI: deferred structured TOC reuse entries=%u",
               (unsigned)out->size());
    }
    return true;
  }

  out->clear();

  if (!markup_utf8.empty() &&
      ParseInlineFileposToc(markup_utf8, text_len_for_pos, callbacks.inline_title,
                            out, reporter)) {
    if (structured_from_filepos)
      *structured_from_filepos = true;
    return true;
  }

  if (source_path.empty() || !callbacks.decode_bytes_to_utf8)
    return false;

  std::string raw;
  if (!file_read_utils::ReadPathToStringLimited(source_path.c_str(), &raw,
                                                kMobiMaxBytes))
    return false;

  std::vector<u32> offsets;
  if (!mobi_record_scan::ParseRecordOffsets(raw, &offsets) || offsets.size() < 3)
    return false;

  const u8 *data = (const u8 *)raw.data();
  const u32 rec0_start = offsets[0];
  const u32 rec0_end = offsets[1];
  if (rec0_end <= rec0_start || rec0_end - rec0_start < 16)
    return false;
  const u8 *rec0 = data + rec0_start;
  const size_t rec0_len = (size_t)(rec0_end - rec0_start);

  mobi_record_decode::MobiRecord0Header rec0_header;
  if (!mobi_record_decode::ParseRecord0Header(rec0, rec0_len, &rec0_header))
    return false;
  if (rec0_header.compression != 1 && rec0_header.compression != 2 &&
      rec0_header.compression != 17480)
    return false;

  u32 max_text_records = (u32)offsets.size() - 2;
  if (rec0_header.text_rec_count == 0 ||
      rec0_header.text_rec_count > max_text_records) {
    rec0_header.text_rec_count = max_text_records;
  }
  if (rec0_header.resource_start > 1) {
    u32 boundary = rec0_header.resource_start - 1;
    if (boundary > 0 && boundary < rec0_header.text_rec_count)
      rec0_header.text_rec_count = boundary;
  }

  if (mobi_structured_toc_parser::ParseStructuredToc(
          raw, offsets, rec0_header.indx_index, rec0_header.encoding,
          callbacks.structured, out, reporter)) {
    return true;
  }

  std::string merged;
  if (!mobi_record_decode::BuildMergedText(raw, offsets, rec0_header, &merged))
    return false;

  std::string utf8 =
      callbacks.decode_bytes_to_utf8(merged, rec0_header.encoding);
  if (ParseInlineFileposToc(
          utf8,
          (rec0_header.text_len > 0) ? rec0_header.text_len : (u32)merged.size(),
          callbacks.inline_title, out, reporter)) {
    if (structured_from_filepos)
      *structured_from_filepos = true;
    return true;
  }

  out->clear();
  return false;
}

} // namespace mobi_toc_resolver

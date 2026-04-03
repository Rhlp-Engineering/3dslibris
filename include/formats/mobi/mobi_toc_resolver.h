/*
    3dslibris - mobi_toc_resolver.h

    MOBI TOC resolver helpers (inline filepos fallback + deferred loading).
*/

#pragma once

#include <string>
#include <vector>

#include <3ds/types.h>

#include "formats/mobi/mobi_structured_toc_parser.h"

class IStatusReporter;

namespace mobi_toc_resolver {

struct InlineTitleCallbacks {
  bool (*looks_like_structured_title)(const std::string &title);
  std::string (*fold_latin_for_match)(const std::string &text);
  size_t (*count_ascii_words)(const std::string &text);
  bool (*is_mostly_digits_or_punctuation)(const std::string &text);
};

struct PrepareCallbacks {
  mobi_structured_toc_parser::ParseCallbacks structured;
  InlineTitleCallbacks inline_title;
  std::string (*decode_bytes_to_utf8)(const std::string &in, u32 encoding);
};

bool ParseInlineFileposToc(const std::string &markup_utf8, u32 text_len,
                           const InlineTitleCallbacks &callbacks,
                           std::vector<mobi_structured_toc_parser::MobiStructuredTocEntry> *out,
                           IStatusReporter *reporter);

bool PrepareStructuredToc(
    const std::string &raw, const std::vector<u32> &offsets, u32 ncx_index,
    u32 encoding, const std::string *markup_utf8, u32 text_len,
    const PrepareCallbacks &callbacks,
    std::vector<mobi_structured_toc_parser::MobiStructuredTocEntry> *out,
    bool *structured_from_filepos, IStatusReporter *reporter);

bool LoadDeferredStructuredToc(
    const std::vector<mobi_structured_toc_parser::MobiStructuredTocEntry> *cached_toc,
    bool have_cached_toc, const std::string &markup_utf8, u32 text_len_for_pos,
    const std::string &source_path, const PrepareCallbacks &callbacks,
    std::vector<mobi_structured_toc_parser::MobiStructuredTocEntry> *out,
    bool *structured_from_filepos, IStatusReporter *reporter);

} // namespace mobi_toc_resolver


/*
    3dslibris - mobi_structured_toc_parser.h

    Structured TOC parser for MOBI INDX/TAGX/CNCX records.
*/

#pragma once

#include <string>
#include <vector>

#include <3ds/types.h>

class IStatusReporter;

namespace mobi_structured_toc_parser {

static const u32 kMobiNullIndex = 0xFFFFFFFFu;

struct MobiStructuredTocEntry {
  std::string title;
  u32 pos; // Raw MOBI filepos offset in merged text stream.
  u8 level;
};

struct ParseCallbacks {
  std::string (*decode_bytes_to_utf8)(const std::string &in, u32 encoding);
  std::string (*normalize_title)(const std::string &in);
  bool (*reject_title)(const std::string &in);
};

bool ParseStructuredToc(const std::string &raw, const std::vector<u32> &offsets,
                        u32 ncx_index, u32 encoding,
                        const ParseCallbacks &callbacks,
                        std::vector<MobiStructuredTocEntry> *out,
                        IStatusReporter *reporter);

} // namespace mobi_structured_toc_parser


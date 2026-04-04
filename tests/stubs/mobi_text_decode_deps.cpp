/*
    Host test stubs for MOBI text decode transitive dependencies.
*/

#include <cstdint>
#include <string>
#include <vector>

typedef std::uint8_t u8;
typedef std::uint16_t u16;
typedef std::uint32_t u32;

class Book {
public:
  Book() : title_() {}
  const char *GetTitle() const { return title_.c_str(); }
  void SetTitle(const char *title) { title_ = title ? title : ""; }
private:
  std::string title_;
};

namespace mobi_parser_core {
struct MobiHeaderInfo {
  u16 compression;
  u32 text_len;
  u32 text_rec_count;
  u32 encoding;
  u32 first_non_book_index;
  u32 mobi_full_name_off;
  u32 mobi_full_name_len;
  u32 huffcdic_record_index;
  u32 num_huffcdic_records;
  u32 trailing_flags;
  u32 ncx_index;
  std::vector<u32> offsets;
  MobiHeaderInfo() : compression(0), text_len(0), text_rec_count(0), encoding(1252),
    first_non_book_index(0), mobi_full_name_off(0), mobi_full_name_len(0),
    huffcdic_record_index(0), num_huffcdic_records(0), trailing_flags(0), ncx_index(0) {}
};

std::string AppendUtf8Codepoint(std::string *out, unsigned int cp) {
  if (!out) return "";
  if (cp < 0x80) { out->push_back((char)cp); }
  else if (cp < 0x800) { out->push_back((char)(0xC0 | (cp >> 6))); out->push_back((char)(0x80 | (cp & 0x3F))); }
  else if (cp < 0x10000) { out->push_back((char)(0xE0 | (cp >> 12))); out->push_back((char)(0x80 | ((cp >> 6) & 0x3F))); out->push_back((char)(0x80 | (cp & 0x3F))); }
  else { out->push_back((char)(0xF0 | (cp >> 18))); out->push_back((char)(0x80 | ((cp >> 12) & 0x3F))); out->push_back((char)(0x80 | ((cp >> 6) & 0x3F))); out->push_back((char)(0x80 | (cp & 0x3F))); }
  return *out;
}
bool LooksLikeValidUtf8Bytes(const std::string &in) {
  for (size_t i = 0; i < in.size(); ) {
    unsigned char c = (unsigned char)in[i];
    if (c < 0x80) { i++; continue; }
    int need = 0;
    if ((c & 0xE0) == 0xC0) need = 2;
    else if ((c & 0xF0) == 0xE0) need = 3;
    else if ((c & 0xF8) == 0xF0) need = 4;
    else return false;
    if (i + (size_t)need > in.size()) return false;
    for (int j = 1; j < need; j++) {
      if (((unsigned char)in[i + j] & 0xC0) != 0x80) return false;
    }
    i += (size_t)need;
  }
  return true;
}
size_t CountUtf8InvalidLeadBytes(const std::string &in) {
  size_t count = 0;
  for (size_t i = 0; i < in.size(); i++) {
    unsigned char c = (unsigned char)in[i];
    if (c >= 0x80 && c < 0xC0) count++;
    else if (c >= 0xF8) count++;
  }
  return count;
}
std::string DecodeLegacySingleByteToUtf8(const std::string &in) {
  static const char *cp1252_extra[32] = {
    "\xE2\x82\xAC", "?", "\xE2\x80\x9A", "\xC6\x92", "\xE2\x80\x9E", "\xE2\x80\xA6",
    "\xE2\x80\xA0", "\xE2\x80\xA1", "\xCB\x86", "\xE2\x80\xB0", "\xC5\xA0", "\xE2\x80\xB9",
    "\xC5\x92", "?", "\xC5\xBD", "?", "?", "\xE2\x80\x98", "\xE2\x80\x99", "\xE2\x80\x9C",
    "\xE2\x80\x9D", "\xE2\x80\xA2", "\xE2\x80\x93", "\xE2\x80\x94", "\xCB\x9C", "\xE2\x84\xA2",
    "\xC5\xA1", "\xE2\x80\xBA", "\xC5\x93", "?", "\xC5\xBE", "\xC5\xB8"
  };
  std::string out;
  out.reserve(in.size());
  for (size_t i = 0; i < in.size(); i++) {
    unsigned char c = (unsigned char)in[i];
    if (c < 0x80) {
      out.push_back((char)c);
    } else if (c >= 0x80 && c < 0xA0) {
      out.append(cp1252_extra[c - 0x80]);
    } else {
      AppendUtf8Codepoint(&out, c);
    }
  }
  return out;
}
std::string DecodeMostlyUtf8WithCp1252Fallback(const std::string &in, unsigned long *) {
  std::string out;
  out.reserve(in.size());
  for (size_t i = 0; i < in.size(); ) {
    unsigned char c = (unsigned char)in[i];
    if (c < 0x80) { out.push_back((char)c); i++; continue; }
    int need = 0;
    if ((c & 0xE0) == 0xC0) need = 2;
    else if ((c & 0xF0) == 0xE0) need = 3;
    else if ((c & 0xF8) == 0xF0) need = 4;
    if (need > 0 && i + (size_t)need <= in.size()) {
      bool valid = true;
      for (int j = 1; j < need; j++) {
        if (((unsigned char)in[i + j] & 0xC0) != 0x80) { valid = false; break; }
      }
      if (valid) { out.append(in, i, (size_t)need); i += (size_t)need; continue; }
    }
    out.append(DecodeLegacySingleByteToUtf8(std::string(1, (char)c)));
    i++;
  }
  return out;
}
}

namespace file_read_utils {
bool ReadPathToStringLimited(const char *, std::string *, unsigned long) { return false; }
}

namespace mobi_record_scan {
bool ParseRecordOffsets(const std::string &, std::vector<u32> *) { return false; }
}

namespace mobi_record_decode {
struct MobiRecord0Header {
  u16 compression;
  u32 text_len;
  u32 text_rec_count;
  u32 encoding;
  u32 first_non_book_index;
  u32 mobi_full_name_off;
  u32 mobi_full_name_len;
  u32 huffcdic_record_index;
  u32 num_huffcdic_records;
  u32 trailing_flags;
  u32 ncx_index;
  MobiRecord0Header() : compression(0), text_len(0), text_rec_count(0), encoding(0),
    first_non_book_index(0), mobi_full_name_off(0), mobi_full_name_len(0),
    huffcdic_record_index(0), num_huffcdic_records(0), trailing_flags(0), ncx_index(0) {}
};
bool ParseRecord0Header(const unsigned char *, unsigned long, MobiRecord0Header *) { return false; }
bool BuildMergedText(const std::string &, const std::vector<u32> &, const MobiRecord0Header &, std::string *) { return false; }
}

static std::string TrimAsciiWhitespace(const std::string &in) {
  size_t start = 0;
  while (start < in.size() && isspace((unsigned char)in[start])) start++;
  size_t end = in.size();
  while (end > start && isspace((unsigned char)in[end - 1])) end--;
  return in.substr(start, end - start);
}

static std::string CollapseAsciiWhitespace(const std::string &in) {
  std::string out;
  out.reserve(in.size());
  bool pending = false;
  for (size_t i = 0; i < in.size(); i++) {
    unsigned char c = (unsigned char)in[i];
    if (isspace(c)) { pending = true; continue; }
    if (pending && !out.empty()) out.push_back(' ');
    pending = false;
    out.push_back((char)c);
  }
  return out;
}

namespace mobi_text_decode {
std::string DecodeBytesToUtf8(const std::string &in, u32 encoding,
                              bool *used_utf8_guess, bool *used_legacy_guess) {
  if (used_utf8_guess) *used_utf8_guess = false;
  if (used_legacy_guess) *used_legacy_guess = false;

  if (encoding == 1200 || encoding == 65002) {
    std::string out;
    if (in.size() < 2) return out;
    bool le = true;
    size_t i = 0;
    if ((unsigned char)in[0] == 0xFE && (unsigned char)in[1] == 0xFF) { le = false; i = 2; }
    else if ((unsigned char)in[0] == 0xFF && (unsigned char)in[1] == 0xFE) { le = true; i = 2; }
    for (; i + 1 < in.size(); ) {
      u16 w = le ? ((u16)(unsigned char)in[i] | ((u16)(unsigned char)in[i+1] << 8))
                 : (((u16)(unsigned char)in[i] << 8) | (u16)(unsigned char)in[i+1]);
      i += 2;
      mobi_parser_core::AppendUtf8Codepoint(&out, w);
    }
    return out;
  }

  const bool raw_is_utf8 = mobi_parser_core::LooksLikeValidUtf8Bytes(in);
  const size_t invalid = raw_is_utf8 ? 0 : mobi_parser_core::CountUtf8InvalidLeadBytes(in);
  const bool mostly_utf8 = !in.empty() && (invalid * 1000 <= in.size() * 2);

  if (encoding == 65001)
    return raw_is_utf8 ? in : mobi_parser_core::DecodeMostlyUtf8WithCp1252Fallback(in, nullptr);

  if (encoding == 0 || encoding == 0xFFFFFFFFu) {
    if (raw_is_utf8) { if (used_utf8_guess) *used_utf8_guess = true; return in; }
    if (mostly_utf8) { if (used_utf8_guess) *used_utf8_guess = true; return mobi_parser_core::DecodeMostlyUtf8WithCp1252Fallback(in, nullptr); }
    if (used_legacy_guess) *used_legacy_guess = true;
    return mobi_parser_core::DecodeLegacySingleByteToUtf8(in);
  }

  if (encoding == 1252) {
    std::string legacy = mobi_parser_core::DecodeLegacySingleByteToUtf8(in);
    // For empty strings, mojibake check returns false, so just return legacy
    return legacy;
  }

  return mobi_parser_core::DecodeLegacySingleByteToUtf8(in);
}

void ApplyEmbeddedTitle(Book *book, const std::string &raw,
                        const mobi_parser_core::MobiHeaderInfo &header) {
  if (!book || header.mobi_full_name_len == 0 || header.mobi_full_name_len >= 2048) return;
  const u8 *data = (const u8 *)raw.data();
  const u32 rec0_start = header.offsets.empty() ? 0 : header.offsets[0];
  const u32 rec0_end = (header.offsets.size() > 1) ? header.offsets[1] : 0;
  if (rec0_end <= rec0_start || rec0_end - rec0_start < 16) return;
  const u8 *rec0 = data + rec0_start;
  const size_t rec0_len = (size_t)(rec0_end - rec0_start);
  if ((size_t)header.mobi_full_name_off + (size_t)header.mobi_full_name_len > rec0_len) return;
  std::string title_raw((const char *)rec0 + header.mobi_full_name_off, (size_t)header.mobi_full_name_len);
  std::string title_utf8 = DecodeBytesToUtf8(title_raw, header.encoding, nullptr, nullptr);
  title_utf8 = CollapseAsciiWhitespace(TrimAsciiWhitespace(title_utf8));
  if (!title_utf8.empty() && title_utf8.size() < 220)
    book->SetTitle(title_utf8.c_str());
}
}

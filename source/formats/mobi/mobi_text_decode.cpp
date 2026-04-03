/*
    3dslibris - mobi_text_decode.cpp
*/

#include "formats/mobi/mobi_text_decode.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "book/book.h"
#include "formats/common/text_helpers.h"

namespace mobi_text_decode {

static bool IsUtf16Encoding(u32 encoding) {
  return encoding == 1200 || encoding == 65002;
}

static bool IsUtf8Encoding(u32 encoding) { return encoding == 65001; }

static bool IsUnknownEncoding(u32 encoding) {
  return encoding == 0 || encoding == 0xFFFFFFFFu;
}

static std::string TrimAsciiWhitespace(const std::string &in) {
  size_t start = 0;
  while (start < in.size() && isspace((unsigned char)in[start]))
    start++;
  size_t end = in.size();
  while (end > start && isspace((unsigned char)in[end - 1]))
    end--;
  return in.substr(start, end - start);
}

static std::string CollapseAsciiWhitespace(const std::string &in) {
  std::string out;
  out.reserve(in.size());
  bool pending_space = false;
  for (size_t i = 0; i < in.size(); i++) {
    unsigned char c = (unsigned char)in[i];
    if (isspace(c)) {
      pending_space = true;
      continue;
    }
    if (pending_space && !out.empty())
      out.push_back(' ');
    pending_space = false;
    out.push_back((char)c);
  }
  return out;
}

static std::string DecodeUtf16ToUtf8(const std::string &in) {
  std::string out;
  if (in.size() < 2)
    return out;

  bool little_endian = true;
  size_t i = 0;
  if ((unsigned char)in[0] == 0xFE && (unsigned char)in[1] == 0xFF) {
    little_endian = false;
    i = 2;
  } else if ((unsigned char)in[0] == 0xFF && (unsigned char)in[1] == 0xFE) {
    little_endian = true;
    i = 2;
  }

  for (; i + 1 < in.size();) {
    u16 w1 =
        little_endian
            ? ((u16)(unsigned char)in[i] | ((u16)(unsigned char)in[i + 1] << 8))
            : (((u16)(unsigned char)in[i] << 8) |
               (u16)(unsigned char)in[i + 1]);
    i += 2;
    u32 cp = w1;
    if (w1 >= 0xD800 && w1 <= 0xDBFF && i + 1 < in.size()) {
      u16 w2 = little_endian ? ((u16)(unsigned char)in[i] |
                                ((u16)(unsigned char)in[i + 1] << 8))
                             : (((u16)(unsigned char)in[i] << 8) |
                                (u16)(unsigned char)in[i + 1]);
      if (w2 >= 0xDC00 && w2 <= 0xDFFF) {
        cp = 0x10000u + (((u32)(w1 - 0xD800) << 10) | (u32)(w2 - 0xDC00));
        i += 2;
      }
    }
    AppendUtf8Codepoint(&out, cp);
  }
  return out;
}

static size_t CountNeedleOccurrences(const std::string &haystack,
                                     const char *needle) {
  if (!needle || !*needle)
    return 0;
  size_t count = 0;
  size_t pos = 0;
  const size_t nlen = strlen(needle);
  while (true) {
    size_t hit = haystack.find(needle, pos);
    if (hit == std::string::npos)
      break;
    count++;
    pos = hit + nlen;
  }
  return count;
}

static size_t CountMojibakeMarkers(const std::string &utf8) {
  size_t score = 0;
  score += CountNeedleOccurrences(utf8, "\xC3\x83");
  score += CountNeedleOccurrences(utf8, "\xC3\x82");
  score += CountNeedleOccurrences(utf8, "\xC3\xA2\xE2\x82\xAC");
  score += CountNeedleOccurrences(utf8, "\xC3\xA2\xE2\x80\x99");
  score += CountNeedleOccurrences(utf8, "\xC3\xA2\xE2\x80\x9C");
  score += CountNeedleOccurrences(utf8, "\xC3\xA2\xE2\x80\x9D");
  return score;
}

static bool LooksLikeMojibakeUtf8Text(const std::string &utf8) {
  return CountMojibakeMarkers(utf8) >= 2;
}

std::string DecodeBytesToUtf8(const std::string &in, u32 encoding,
                              bool *used_utf8_guess,
                              bool *used_legacy_guess) {
  if (used_utf8_guess)
    *used_utf8_guess = false;
  if (used_legacy_guess)
    *used_legacy_guess = false;

  if (IsUtf16Encoding(encoding))
    return DecodeUtf16ToUtf8(in);

  const bool raw_is_utf8 = LooksLikeValidUtf8Bytes(in);
  const size_t invalid_utf8_leads =
      raw_is_utf8 ? 0 : CountUtf8InvalidLeadBytes(in);
  const bool mostly_utf8 =
      !in.empty() && (invalid_utf8_leads * 1000 <= in.size() * 2);
  if (IsUtf8Encoding(encoding))
    return raw_is_utf8 ? in : DecodeMostlyUtf8WithCp1252Fallback(in, NULL);

  if (IsUnknownEncoding(encoding)) {
    if (raw_is_utf8) {
      if (used_utf8_guess)
        *used_utf8_guess = true;
      return in;
    }
    if (mostly_utf8) {
      if (used_utf8_guess)
        *used_utf8_guess = true;
      return DecodeMostlyUtf8WithCp1252Fallback(in, NULL);
    }
    if (used_legacy_guess)
      *used_legacy_guess = true;
    return DecodeLegacySingleByteToUtf8(in);
  }

  if (encoding == 1252) {
    std::string legacy_candidate = DecodeLegacySingleByteToUtf8(in);
    if ((raw_is_utf8 || mostly_utf8) &&
        LooksLikeMojibakeUtf8Text(legacy_candidate)) {
      if (raw_is_utf8) {
        if (used_utf8_guess)
          *used_utf8_guess = true;
        return in;
      }
      std::string mixed_utf8 = DecodeMostlyUtf8WithCp1252Fallback(in, NULL);
      const size_t legacy_mojibake = CountMojibakeMarkers(legacy_candidate);
      const size_t mixed_mojibake = CountMojibakeMarkers(mixed_utf8);
      if (legacy_mojibake >= 2 && mixed_mojibake + 1 < legacy_mojibake) {
        if (used_utf8_guess)
          *used_utf8_guess = true;
        return mixed_utf8;
      }
    }
    return legacy_candidate;
  }

  std::string legacy_candidate = DecodeLegacySingleByteToUtf8(in);
  if ((raw_is_utf8 || mostly_utf8) &&
      LooksLikeMojibakeUtf8Text(legacy_candidate)) {
    if (raw_is_utf8) {
      if (used_utf8_guess)
        *used_utf8_guess = true;
      return in;
    }
    std::string mixed_utf8 = DecodeMostlyUtf8WithCp1252Fallback(in, NULL);
    const size_t legacy_mojibake = CountMojibakeMarkers(legacy_candidate);
    const size_t mixed_mojibake = CountMojibakeMarkers(mixed_utf8);
    if (legacy_mojibake >= 2 && mixed_mojibake + 1 < legacy_mojibake) {
      if (used_utf8_guess)
        *used_utf8_guess = true;
      return mixed_utf8;
    }
  }
  return legacy_candidate;
}

void ApplyEmbeddedTitle(Book *book, const std::string &raw,
                        const mobi_parser_core::MobiHeaderInfo &header) {
  if (!book || header.mobi_full_name_len == 0 || header.mobi_full_name_len >= 2048)
    return;

  const u8 *data = (const u8 *)raw.data();
  const u32 rec0_start = header.offsets.empty() ? 0 : header.offsets[0];
  const u32 rec0_end = (header.offsets.size() > 1) ? header.offsets[1] : 0;
  if (rec0_end <= rec0_start || rec0_end - rec0_start < 16)
    return;

  const u8 *rec0 = data + rec0_start;
  const size_t rec0_len = (size_t)(rec0_end - rec0_start);
  std::string title_raw;
  bool title_ok = false;
  if ((size_t)header.mobi_full_name_off + (size_t)header.mobi_full_name_len <=
      rec0_len) {
    title_raw.assign((const char *)rec0 + header.mobi_full_name_off,
                     (size_t)header.mobi_full_name_len);
    title_ok = true;
  } else if ((size_t)16 + (size_t)header.mobi_full_name_off +
                 (size_t)header.mobi_full_name_len <=
             rec0_len) {
    title_raw.assign((const char *)rec0 + 16 + (size_t)header.mobi_full_name_off,
                     (size_t)header.mobi_full_name_len);
    title_ok = true;
  }
  if (!title_ok)
    return;

  std::string title_utf8 =
      DecodeBytesToUtf8(title_raw, header.encoding, NULL, NULL);
  title_utf8 = CollapseAsciiWhitespace(TrimAsciiWhitespace(title_utf8));
  if (!title_utf8.empty() && title_utf8.size() < 220)
    book->SetTitle(title_utf8.c_str());
}

} // namespace mobi_text_decode

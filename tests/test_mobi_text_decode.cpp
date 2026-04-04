#include "test_assert.h"

#include <cstdint>
#include <string>
#include <vector>

typedef std::uint8_t u8;
typedef std::uint16_t u16;
typedef std::uint32_t u32;

class Book {
public:
  Book() : title_() {}

  const char *GetTitle() { return title_.c_str(); }
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

  MobiHeaderInfo();
};

MobiHeaderInfo::MobiHeaderInfo()
    : compression(0), text_len(0), text_rec_count(0), encoding(1252),
      first_non_book_index(0), mobi_full_name_off(0), mobi_full_name_len(0),
      huffcdic_record_index(0), num_huffcdic_records(0), trailing_flags(0),
      ncx_index(0) {}

}

namespace mobi_text_decode {

std::string DecodeBytesToUtf8(const std::string &in, u32 encoding,
                              bool *used_utf8_guess,
                              bool *used_legacy_guess);

void ApplyEmbeddedTitle(Book *book, const std::string &raw,
                        const mobi_parser_core::MobiHeaderInfo &header);

}

namespace {

void TestDecodeCp1252() {
  const std::string raw = std::string("caf") + "\xE9" + " y " + "\x97" + " fin";
  bool used_utf8_guess = true;
  bool used_legacy_guess = true;
  const std::string decoded =
      mobi_text_decode::DecodeBytesToUtf8(raw, 1252, &used_utf8_guess,
                                          &used_legacy_guess);

  test::ExpectStrEq("cp1252 decode", decoded.c_str(), "caf\xC3\xA9 y \xE2\x80\x94 fin");
  test::ExpectFalse("cp1252 used_utf8_guess", used_utf8_guess);
  test::ExpectFalse("cp1252 used_legacy_guess", used_legacy_guess);
}

void TestUtf8DetectionAndPassThrough() {
  const std::string utf8 = "\xC2\xA1Hola, se\xC3\xB1or!";

  bool used_utf8_guess = false;
  bool used_legacy_guess = false;
  const std::string detected =
      mobi_text_decode::DecodeBytesToUtf8(utf8, 0, &used_utf8_guess,
                                          &used_legacy_guess);
  test::ExpectStrEq("unknown encoding utf8 passthrough", detected.c_str(),
                    utf8.c_str());
  test::ExpectTrue("unknown encoding used_utf8_guess", used_utf8_guess);
  test::ExpectFalse("unknown encoding used_legacy_guess", used_legacy_guess);

  used_utf8_guess = true;
  used_legacy_guess = true;
  const std::string explicit_utf8 =
      mobi_text_decode::DecodeBytesToUtf8(utf8, 65001, &used_utf8_guess,
                                          &used_legacy_guess);
  test::ExpectStrEq("explicit utf8 passthrough", explicit_utf8.c_str(),
                    utf8.c_str());
  test::ExpectFalse("explicit utf8 used_utf8_guess", used_utf8_guess);
  test::ExpectFalse("explicit utf8 used_legacy_guess", used_legacy_guess);
}

void TestDecodeIso88591() {
  const std::string raw = std::string("Ol") + "\xE1" + " mundo";
  bool used_utf8_guess = true;
  bool used_legacy_guess = true;
  const std::string decoded =
      mobi_text_decode::DecodeBytesToUtf8(raw, 28591, &used_utf8_guess,
                                          &used_legacy_guess);

  test::ExpectStrEq("iso-8859-1 decode", decoded.c_str(), "Ol\xC3\xA1 mundo");
  test::ExpectFalse("iso-8859-1 used_utf8_guess", used_utf8_guess);
  test::ExpectFalse("iso-8859-1 used_legacy_guess", used_legacy_guess);
}

void TestApplyEmbeddedTitleFromMetadata() {
  mobi_parser_core::MobiHeaderInfo header;
  header.encoding = 1252;
  header.mobi_full_name_off = 12;
  const std::string raw_title = std::string("  T") + "\xED" + "tulo   de   prueba  ";
  header.mobi_full_name_len = static_cast<u32>(raw_title.size());

  std::string rec0(80, '\0');
  rec0.replace(header.mobi_full_name_off, raw_title.size(), raw_title);

  std::string raw = rec0;
  raw.append("NEXT_RECORD");

  header.offsets.push_back(0);
  header.offsets.push_back(static_cast<u32>(rec0.size()));
  header.offsets.push_back(static_cast<u32>(raw.size()));

  Book book;
  mobi_text_decode::ApplyEmbeddedTitle(&book, raw, header);
  test::ExpectStrEq("embedded title extracted and normalized", book.GetTitle(),
                    "T\xC3\xADtulo de prueba");
}

void TestUnknownEncodingWithMalformedBytes() {
  const std::string malformed = std::string("A") + "\xFF\x80";
  bool used_utf8_guess = false;
  bool used_legacy_guess = false;

  const std::string decoded =
      mobi_text_decode::DecodeBytesToUtf8(malformed, 0xFFFFFFFFu,
                                          &used_utf8_guess,
                                          &used_legacy_guess);

  test::ExpectTrue("malformed unknown picks legacy guess", used_legacy_guess);
  test::ExpectFalse("malformed unknown utf8 guess", used_utf8_guess);
  test::ExpectStrContains("malformed unknown keeps ascii", decoded.c_str(), "A");
  test::ExpectStrContains("malformed unknown maps cp1252 euro", decoded.c_str(),
                          "\xE2\x82\xAC");
}

void TestNullAndEmptyInputHandling() {
  bool used_utf8_guess = true;
  bool used_legacy_guess = true;
  const std::string decoded =
      mobi_text_decode::DecodeBytesToUtf8("", 1252, &used_utf8_guess,
                                          &used_legacy_guess);
  test::ExpectStrEq("empty decode", decoded.c_str(), "");
  test::ExpectFalse("empty decode utf8 guess", used_utf8_guess);
  test::ExpectFalse("empty decode legacy guess", used_legacy_guess);

  mobi_parser_core::MobiHeaderInfo header;
  header.mobi_full_name_len = 10;
  Book book;
  book.SetTitle("unchanged");
  mobi_text_decode::ApplyEmbeddedTitle(nullptr, "", header);
  mobi_text_decode::ApplyEmbeddedTitle(&book, "", header);
  test::ExpectStrEq("null/empty title apply leaves title unchanged",
                    book.GetTitle(), "unchanged");
}

}

int main() {
  TestDecodeCp1252();
  TestUtf8DetectionAndPassThrough();
  TestDecodeIso88591();
  TestApplyEmbeddedTitleFromMetadata();
  TestUnknownEncodingWithMalformedBytes();
  TestNullAndEmptyInputHandling();
  return 0;
}

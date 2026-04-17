#include "formats/common/html_entity_utils.h"
#include "test_assert.h"

#include <string>

namespace {

void TestDecodesPortugueseNamedEntities() {
  uint32_t cp = 0;
  test::ExpectTrue("atilde decodes",
                   html_entity_utils::DecodeHtmlEntityCodepoint("atilde", &cp));
  test::ExpectEq("atilde codepoint", (int)cp, 0x00E3);

  test::ExpectTrue("uacute decodes",
                   html_entity_utils::DecodeHtmlEntityCodepoint("&uacute;", &cp));
  test::ExpectEq("uacute codepoint", (int)cp, 0x00FA);

  test::ExpectTrue("agrave decodes",
                   html_entity_utils::DecodeHtmlEntityCodepoint("agrave", &cp));
  test::ExpectEq("agrave codepoint", (int)cp, 0x00E0);

  test::ExpectTrue("ocirc decodes",
                   html_entity_utils::DecodeHtmlEntityCodepoint("ocirc", &cp));
  test::ExpectEq("ocirc codepoint", (int)cp, 0x00F4);
}

void TestDecodesPunctuationEntities() {
  uint32_t cp = 0;
  test::ExpectTrue("ndash decodes",
                   html_entity_utils::DecodeHtmlEntityCodepoint("ndash", &cp));
  test::ExpectEq("ndash codepoint", (int)cp, 0x2013);

  test::ExpectTrue("hellip decodes",
                   html_entity_utils::DecodeHtmlEntityCodepoint("hellip", &cp));
  test::ExpectEq("hellip codepoint", (int)cp, 0x2026);
}

void TestDecodesNumericEntities() {
  uint32_t cp = 0;
  test::ExpectTrue("decimal entity",
                   html_entity_utils::DecodeHtmlEntityCodepoint("&#227;", &cp));
  test::ExpectEq("decimal codepoint", (int)cp, 0x00E3);

  test::ExpectTrue("hex entity",
                   html_entity_utils::DecodeHtmlEntityCodepoint("&#x2013;", &cp));
  test::ExpectEq("hex codepoint", (int)cp, 0x2013);
}

void TestEncodesUtf8Output() {
  std::string out;
  test::ExpectTrue("utf8 entity output",
                   html_entity_utils::DecodeHtmlEntityUtf8("ccedil", &out));
  test::ExpectStrEq("utf8 ccedil", out.c_str(), "\xC3\xA7");
}

void TestRejectsInvalidEntities() {
  uint32_t cp = 0;
  test::ExpectFalse("unknown entity",
                    html_entity_utils::DecodeHtmlEntityCodepoint("nope", &cp));
  test::ExpectFalse("invalid scalar",
                    html_entity_utils::DecodeHtmlEntityCodepoint("&#0;", &cp));
  test::ExpectFalse(
      "surrogate scalar",
      html_entity_utils::DecodeHtmlEntityCodepoint("&#xD800;", &cp));
}

void TestNormalizesNamedEntitiesForXml() {
  const std::string normalized = html_entity_utils::NormalizeHtmlNamedEntitiesForXml(
      "Portugu&ecirc;s &amp; cora&ccedil;&atilde;o &ndash; &#227;");
  test::ExpectStrEq("named entities normalized", normalized.c_str(),
                    "Portugu&#234;s &amp; cora&#231;&#227;o &#8211; &#227;");
}

void TestNormalizesNamedEntitiesAcrossChunks() {
  html_entity_utils::ChunkedEntityNormalizerState state;
  std::string out;
  html_entity_utils::NormalizeHtmlNamedEntitiesForXmlChunk(
      "&ec", false, &state, &out);
  html_entity_utils::NormalizeHtmlNamedEntitiesForXmlChunk(
      "irc; e &nd", false, &state, &out);
  html_entity_utils::NormalizeHtmlNamedEntitiesForXmlChunk(
      "ash;", true, &state, &out);
  test::ExpectStrEq("chunked entities normalized", out.c_str(),
                    "&#234; e &#8211;");
}

} // namespace

int main() {
  TestDecodesPortugueseNamedEntities();
  TestDecodesPunctuationEntities();
  TestDecodesNumericEntities();
  TestEncodesUtf8Output();
  TestRejectsInvalidEntities();
  TestNormalizesNamedEntitiesForXml();
  TestNormalizesNamedEntitiesAcrossChunks();
  return 0;
}

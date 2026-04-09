#include "book/book_xml_css_style_utils.h"

#include "test_assert.h"

namespace {

void TestDetectsOverlineDecoration() {
  book_xml_css_style_utils::InlineStyleFlags flags{};
  book_xml_css_style_utils::ParseInlineStyleFlags(
      "text-decoration: overline;", &flags);

  test::ExpectTrue("overline detected", flags.overline);
  test::ExpectFalse("underline not implied", flags.underline);
}

void TestDetectsUnderlineRegardlessOfDecorationOrder() {
  book_xml_css_style_utils::InlineStyleFlags flags{};
  book_xml_css_style_utils::ParseInlineStyleFlags(
      "text-decoration: wavy underline;", &flags);

  test::ExpectTrue("underline detected", flags.underline);
  test::ExpectEq("wavy underline style", (int)flags.underline_style,
                 UNDERLINE_STYLE_WAVY);
  test::ExpectFalse("strikethrough not implied", flags.strikethrough);
}

void TestDetectsDashedAndDottedUnderlineStyles() {
  book_xml_css_style_utils::InlineStyleFlags dashed{};
  book_xml_css_style_utils::ParseInlineStyleFlags(
      "text-decoration: underline dashed;", &dashed);
  test::ExpectTrue("dashed underline detected", dashed.underline);
  test::ExpectEq("dashed underline style", (int)dashed.underline_style,
                 UNDERLINE_STYLE_DASHED);

  book_xml_css_style_utils::InlineStyleFlags dotted{};
  book_xml_css_style_utils::ParseInlineStyleFlags(
      "text-decoration-style: dotted; text-decoration-line: underline;",
      &dotted);
  test::ExpectTrue("dotted underline detected", dotted.underline);
  test::ExpectEq("dotted underline style", (int)dotted.underline_style,
                 UNDERLINE_STYLE_DOTTED);
}

void TestDetectsBoldItalicAndVerticalAlign() {
  book_xml_css_style_utils::InlineStyleFlags flags{};
  book_xml_css_style_utils::ParseInlineStyleFlags(
      "font-weight: bold; font-style: italic; vertical-align: super;", &flags);

  test::ExpectTrue("bold detected", flags.bold);
  test::ExpectTrue("italic detected", flags.italic);
  test::ExpectTrue("superscript detected", flags.superscript);
  test::ExpectFalse("subscript not implied", flags.subscript);
}

} // namespace

int main() {
  TestDetectsOverlineDecoration();
  TestDetectsUnderlineRegardlessOfDecorationOrder();
  TestDetectsDashedAndDottedUnderlineStyles();
  TestDetectsBoldItalicAndVerticalAlign();
  return 0;
}

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

void TestParseMarginTopPositivePx() {
  using R = book_xml_css_style_utils::MarginTopResult;
  R r = book_xml_css_style_utils::ParseMarginTop("margin-top: 20px;");
  test::ExpectEq("px unit",  (int)r.unit, (int)R::Unit::Px);
  test::ExpectEq("px value", r.value,     20);
  test::ExpectFalse("not negative", r.negative);
}

void TestParseMarginTopNegativePx() {
  using R = book_xml_css_style_utils::MarginTopResult;
  R r = book_xml_css_style_utils::ParseMarginTop("margin-top: -10px;");
  test::ExpectEq("px unit",    (int)r.unit, (int)R::Unit::Px);
  test::ExpectEq("px value",   r.value,     10);
  test::ExpectTrue("negative flag", r.negative);
}

void TestParseMarginTopPercent() {
  using R = book_xml_css_style_utils::MarginTopResult;
  R r = book_xml_css_style_utils::ParseMarginTop("margin-top: 5%;");
  test::ExpectEq("percent unit",  (int)r.unit, (int)R::Unit::Percent);
  test::ExpectEq("percent value", r.value,     5);
  test::ExpectFalse("not negative", r.negative);
}

void TestParseMarginTopZeroUnitless() {
  using R = book_xml_css_style_utils::MarginTopResult;
  R r = book_xml_css_style_utils::ParseMarginTop("margin-top: 0;");
  test::ExpectEq("zero unitless -> None", (int)r.unit, (int)R::Unit::None);
}

void TestParseMarginTopZeroPx() {
  using R = book_xml_css_style_utils::MarginTopResult;
  R r = book_xml_css_style_utils::ParseMarginTop("margin-top: 0px;");
  test::ExpectEq("zero px unit",  (int)r.unit, (int)R::Unit::Px);
  test::ExpectEq("zero px value", r.value,     0);
}

void TestParseMarginTopMissingProperty() {
  using R = book_xml_css_style_utils::MarginTopResult;
  R r = book_xml_css_style_utils::ParseMarginTop("font-weight: bold;");
  test::ExpectEq("missing -> None", (int)r.unit, (int)R::Unit::None);
}

void TestParseMarginTopNull() {
  using R = book_xml_css_style_utils::MarginTopResult;
  R r = book_xml_css_style_utils::ParseMarginTop(nullptr);
  test::ExpectEq("null -> None", (int)r.unit, (int)R::Unit::None);
}

void TestParseMarginTopEmUnit() {
  using R = book_xml_css_style_utils::MarginTopResult;
  R r = book_xml_css_style_utils::ParseMarginTop("margin-top: 2em;");
  test::ExpectEq("em -> None", (int)r.unit, (int)R::Unit::None);
}

} // namespace

int main() {
  TestDetectsOverlineDecoration();
  TestDetectsUnderlineRegardlessOfDecorationOrder();
  TestDetectsDashedAndDottedUnderlineStyles();
  TestDetectsBoldItalicAndVerticalAlign();
  TestParseMarginTopPositivePx();
  TestParseMarginTopNegativePx();
  TestParseMarginTopPercent();
  TestParseMarginTopZeroUnitless();
  TestParseMarginTopZeroPx();
  TestParseMarginTopMissingProperty();
  TestParseMarginTopNull();
  TestParseMarginTopEmUnit();
  return 0;
}

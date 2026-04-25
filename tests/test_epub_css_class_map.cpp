#include "book/epub_css_class_map.h"

#include "test_assert.h"

namespace {

using epub_css_class_map::CssClassMap;
using epub_css_class_map::CssClassMargins;

void ExpectMarginTopPx(const char *label, const CssClassMargins &m, int value) {
  test::ExpectEq((std::string(label) + " top unit").c_str(),
                 (int)m.margin_top.unit,
                 (int)book_xml_css_style_utils::MarginTopResult::Unit::Px);
  test::ExpectEq((std::string(label) + " top value").c_str(),
                 m.margin_top.value, value);
}

void ExpectMarginBottomPercent(const char *label, const CssClassMargins &m,
                               int value) {
  test::ExpectEq((std::string(label) + " bottom unit").c_str(),
                 (int)m.margin_bottom.unit,
                 (int)book_xml_css_style_utils::MarginTopResult::Unit::Percent);
  test::ExpectEq((std::string(label) + " bottom value").c_str(),
                 m.margin_bottom.value, value);
}

void TestParseCssIntoClassMapSupportsQualifiedAndGroupedSelectors() {
  const char *css =
      ".mt-inline { margin-top: 24px; }\n"
      "p.mb-class { margin-bottom: 8%; }\n"
      ".align-center { text-align: center; }\n"
      ".mt-group, h2.mt-heading { margin-top: 12px; }\n"
      "div.note strong { margin-top: 99px; }\n"
      ".combo.one { margin-bottom: 77px; }\n";

  CssClassMap out;
  epub_css_class_map::ParseCssIntoClassMap(css, std::strlen(css), &out);

  test::ExpectEq("parsed class count", (int)out.size(), 5);
  test::ExpectTrue("simple class parsed", out.find("mt-inline") != out.end());
  test::ExpectTrue("element qualified class parsed",
                   out.find("mb-class") != out.end());
  test::ExpectTrue("grouped selector class parsed",
                   out.find("mt-group") != out.end());
  test::ExpectTrue("grouped selector qualified class parsed",
                   out.find("mt-heading") != out.end());
  test::ExpectTrue("align class parsed", out.find("align-center") != out.end());
  test::ExpectTrue("descendant selector ignored",
                   out.find("note") == out.end());
  test::ExpectTrue("compound class selector ignored",
                   out.find("combo") == out.end() && out.find("one") == out.end());

  ExpectMarginTopPx("mt-inline", out["mt-inline"], 24);
  ExpectMarginBottomPercent("mb-class", out["mb-class"], 8);
  ExpectMarginTopPx("mt-group", out["mt-group"], 12);
  ExpectMarginTopPx("mt-heading", out["mt-heading"], 12);
  test::ExpectTrue("align-center keeps text-align",
                   out["align-center"].has_text_align);
  test::ExpectEq("align-center value",
                 (int)out["align-center"].text_align,
                 (int)book_xml_css_style_utils::TextAlign::Center);
}

void TestLookupMarginsForClassAttrMergesKnownClasses() {
  CssClassMap map;
  map["mt-class"].margin_top.value = 18;
  map["mt-class"].margin_top.unit =
      book_xml_css_style_utils::MarginTopResult::Unit::Px;
  map["mb-class"].margin_bottom.value = 6;
  map["mb-class"].margin_bottom.unit =
      book_xml_css_style_utils::MarginTopResult::Unit::Percent;

  CssClassMargins out;
  const bool found = epub_css_class_map::LookupMarginsForClassAttr(
      "  ignored mt-class  mb-class ", map, &out);

  test::ExpectTrue("found at least one known class", found);
  ExpectMarginTopPx("merged top", out, 18);
  ExpectMarginBottomPercent("merged bottom", out, 6);
}

void TestLookupMarginsForClassAttrRejectsUnknownClasses() {
  CssClassMap map;
  CssClassMargins out;
  const bool found = epub_css_class_map::LookupMarginsForClassAttr(
      "foo bar", map, &out);
  test::ExpectFalse("no classes found", found);
}

void TestParseCssIntoClassMapDetectsListStyleNone() {
  const char *css =
      ".ornamentless { list-style-type: none; }\n"
      "ol.clean { list-style: none; }\n"
      ".normal-list { list-style-type: disc; }\n";

  CssClassMap out;
  epub_css_class_map::ParseCssIntoClassMap(css, std::strlen(css), &out);

  test::ExpectTrue("ornamentless parsed", out.find("ornamentless") != out.end());
  test::ExpectTrue("qualified clean parsed", out.find("clean") != out.end());
  test::ExpectTrue("ornamentless hides markers",
                   out["ornamentless"].hide_list_markers);
  test::ExpectTrue("clean hides markers", out["clean"].hide_list_markers);
  test::ExpectFalse("normal list keeps markers",
                    out["normal-list"].hide_list_markers);
}

void TestLookupTextAlignForClassAttrUsesLastKnownMatch() {
  CssClassMap map;
  map["align-left"].has_text_align = true;
  map["align-left"].text_align = book_xml_css_style_utils::TextAlign::Left;
  map["align-right"].has_text_align = true;
  map["align-right"].text_align = book_xml_css_style_utils::TextAlign::Right;

  book_xml_css_style_utils::TextAlign align =
      book_xml_css_style_utils::TextAlign::Center;
  const bool found = epub_css_class_map::LookupTextAlignForClassAttr(
      "align-left align-right", map, &align);
  test::ExpectTrue("found text align", found);
  test::ExpectEq("last matching class wins", (int)align,
                 (int)book_xml_css_style_utils::TextAlign::Right);
}

} // namespace

int main() {
  TestParseCssIntoClassMapSupportsQualifiedAndGroupedSelectors();
  TestLookupMarginsForClassAttrMergesKnownClasses();
  TestLookupMarginsForClassAttrRejectsUnknownClasses();
  TestParseCssIntoClassMapDetectsListStyleNone();
  TestLookupTextAlignForClassAttrUsesLastKnownMatch();
  return 0;
}

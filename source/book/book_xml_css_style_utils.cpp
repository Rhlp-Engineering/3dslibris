#include "book/book_xml_css_style_utils.h"

#include "shared/string_utils.h"

#include <string>

namespace book_xml_css_style_utils {

namespace {

bool ContainsNoCase(const std::string &haystack, const char *needle) {
  return needle && needle[0] &&
         haystack.find(ToLowerAscii(std::string(needle))) != std::string::npos;
}

bool HasTextDecorationKeyword(const std::string &style_lc,
                              const char *keyword) {
  if (!keyword || !keyword[0])
    return false;
  const bool mentions_text_decoration =
      style_lc.find("text-decoration") != std::string::npos ||
      style_lc.find("text-decoration-line") != std::string::npos;
  return mentions_text_decoration &&
         style_lc.find(keyword) != std::string::npos;
}

unsigned char ParseUnderlineStyle(const std::string &style_lc) {
  const bool mentions_text_decoration =
      style_lc.find("text-decoration") != std::string::npos ||
      style_lc.find("text-decoration-line") != std::string::npos ||
      style_lc.find("text-decoration-style") != std::string::npos;
  if (!mentions_text_decoration)
    return UNDERLINE_STYLE_SOLID;
  if (style_lc.find("wavy") != std::string::npos)
    return UNDERLINE_STYLE_WAVY;
  if (style_lc.find("dashed") != std::string::npos)
    return UNDERLINE_STYLE_DASHED;
  if (style_lc.find("dotted") != std::string::npos)
    return UNDERLINE_STYLE_DOTTED;
  return UNDERLINE_STYLE_SOLID;
}

} // namespace

void ParseInlineStyleFlags(const char *style, InlineStyleFlags *out) {
  if (!style || !out)
    return;

  const std::string style_lc = ToLowerAscii(std::string(style));

  if (ContainsNoCase(style_lc, "font-style:italic") ||
      ContainsNoCase(style_lc, "font-style: italic") ||
      ContainsNoCase(style_lc, "font-style:oblique") ||
      ContainsNoCase(style_lc, "font-style: oblique") ||
      ContainsNoCase(style_lc, "font:italic") ||
      ContainsNoCase(style_lc, "font: italic") ||
      ContainsNoCase(style_lc, "font:oblique") ||
      ContainsNoCase(style_lc, "font: oblique")) {
    out->italic = true;
  }

  if (ContainsNoCase(style_lc, "font-weight:bold") ||
      ContainsNoCase(style_lc, "font-weight: bold") ||
      ContainsNoCase(style_lc, "font-weight:bolder") ||
      ContainsNoCase(style_lc, "font-weight: bolder") ||
      ContainsNoCase(style_lc, "font-weight:600") ||
      ContainsNoCase(style_lc, "font-weight: 600") ||
      ContainsNoCase(style_lc, "font-weight:700") ||
      ContainsNoCase(style_lc, "font-weight: 700") ||
      ContainsNoCase(style_lc, "font-weight:800") ||
      ContainsNoCase(style_lc, "font-weight: 800") ||
      ContainsNoCase(style_lc, "font-weight:900") ||
      ContainsNoCase(style_lc, "font-weight: 900") ||
      ContainsNoCase(style_lc, "font:bold") ||
      ContainsNoCase(style_lc, "font: bold")) {
    out->bold = true;
  }

  if (HasTextDecorationKeyword(style_lc, "underline")) {
    out->underline = true;
    out->underline_style = ParseUnderlineStyle(style_lc);
  }
  if (HasTextDecorationKeyword(style_lc, "overline"))
    out->overline = true;
  if (HasTextDecorationKeyword(style_lc, "line-through"))
    out->strikethrough = true;

  if (ContainsNoCase(style_lc, "vertical-align:super") ||
      ContainsNoCase(style_lc, "vertical-align: super")) {
    out->superscript = true;
  }

  if (ContainsNoCase(style_lc, "vertical-align:sub") ||
      ContainsNoCase(style_lc, "vertical-align: sub")) {
    out->subscript = true;
  }
}

} // namespace book_xml_css_style_utils

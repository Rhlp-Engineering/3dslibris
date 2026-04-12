#pragma once

#include "book/book_xml_css_style_utils.h"
#include "parse.h"
#include "screen_constants.h"
#include "shared/text_token_constants.h"

#include <algorithm>

namespace book_xml_parser_style_utils {

inline void EmitUnderlineStyleMarker(parsedata_t *p, u8 underline_style) {
  if (!p)
    return;
  parse_append_page_byte(p, TEXT_UNDERLINE_STYLE);
  parse_append_page_byte(p, underline_style);
}

inline u8 ResolveParsedTextStyle(bool bold, bool italic, bool mono) {
  if (mono && bold && italic)
    return TEXT_STYLE_MONO_BOLDITALIC;
  if (mono && bold)
    return TEXT_STYLE_MONO_BOLD;
  if (mono && italic)
    return TEXT_STYLE_MONO_ITALIC;
  if (mono)
    return TEXT_STYLE_MONO;
  if (bold && italic)
    return TEXT_STYLE_BOLDITALIC;
  if (bold)
    return TEXT_STYLE_BOLD;
  if (italic)
    return TEXT_STYLE_ITALIC;
  return TEXT_STYLE_REGULAR;
}

inline void RestoreParsedStyleMarkers(parsedata_t *p) {
  if (!p)
    return;
  if (parse_in(p, TAG_PRE))
    parse_append_page_byte(p, TEXT_PRE_ON);
  if (p->superscript)
    parse_append_page_byte(p, TEXT_SUPERSCRIPT_ON);
  if (p->subscript)
    parse_append_page_byte(p, TEXT_SUBSCRIPT_ON);
  if (p->strikethrough)
    parse_append_page_byte(p, TEXT_STRIKETHROUGH_ON);
  if (p->overline)
    parse_append_page_byte(p, TEXT_OVERLINE_ON);
  if (p->underline) {
    parse_append_page_byte(p, TEXT_UNDERLINE_ON);
    EmitUnderlineStyleMarker(p, p->underline_style);
  }
  if (p->italic)
    parse_append_page_byte(p, TEXT_ITALIC_ON);
  if (p->bold)
    parse_append_page_byte(p, TEXT_BOLD_ON);
  if (p->mono)
    parse_append_page_byte(p, TEXT_MONO_ON);
}

inline int ResolveCssMarginLinefeeds(
    const book_xml_css_style_utils::MarginTopResult &m, int line_h) {
  using Unit = book_xml_css_style_utils::MarginTopResult::Unit;
  if (m.unit == Unit::None || line_h <= 0)
    return 0;
  int px = m.value;
  if (m.unit == Unit::Percent)
    px = (m.value * PAGE_WIDTH) / 100;
  if (m.negative || px <= 0)
    return 0;
  const int linefeeds = (px + line_h - 1) / line_h;
  return std::max(1, std::min(linefeeds, 4));
}

inline int ClampResolvedBlockLinefeeds(int linefeeds) {
  return std::max(0, std::min(linefeeds, 6));
}

inline int ResolveBlockTopLinefeeds(
    int default_lf, const book_xml_css_style_utils::MarginTopResult &m,
    int line_h) {
  using Unit = book_xml_css_style_utils::MarginTopResult::Unit;
  if (m.unit == Unit::None)
    return ClampResolvedBlockLinefeeds(default_lf);
  if (m.negative || (m.unit != Unit::None && m.value == 0))
    return 0;
  const int css_lf = ResolveCssMarginLinefeeds(m, line_h);
  return ClampResolvedBlockLinefeeds(std::max(css_lf, default_lf + 1));
}

inline int ResolveBlockBottomLinefeeds(
    int default_lf, const book_xml_css_style_utils::MarginTopResult &m,
    int line_h) {
  using Unit = book_xml_css_style_utils::MarginTopResult::Unit;
  if (m.unit == Unit::None)
    return ClampResolvedBlockLinefeeds(default_lf);
  if (m.negative || (m.unit != Unit::None && m.value == 0))
    return 0;
  const int css_lf = ResolveCssMarginLinefeeds(m, line_h);
  return ClampResolvedBlockLinefeeds(std::max(css_lf, default_lf + 1));
}

} // namespace book_xml_parser_style_utils

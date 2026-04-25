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

inline void RestoreParsedParagraphAlignmentMarker(parsedata_t *p) {
  if (!p)
    return;

  book_xml_css_style_utils::TextAlign align =
      book_xml_css_style_utils::TextAlign::Left;
  bool have_align = false;
  bool in_alignable_block = false;

  if (parse_in(p, TAG_P)) {
    in_alignable_block = true;
    have_align = book_xml_css_style_utils::TryParseTextAlign(
        p->last_p_style.c_str(), &align);
    if (!have_align) {
      have_align = epub_css_class_map::LookupTextAlignForClassAttr(
          p->last_p_class, p->css_class_map, &align);
    }
  } else if (parse_in(p, TAG_H1)) {
    in_alignable_block = true;
    have_align = book_xml_css_style_utils::TryParseTextAlign(
        p->last_h1_style.c_str(), &align);
    if (!have_align) {
      have_align = epub_css_class_map::LookupTextAlignForClassAttr(
          p->last_h1_class, p->css_class_map, &align);
    }
  } else if (parse_in(p, TAG_H2)) {
    in_alignable_block = true;
    have_align = book_xml_css_style_utils::TryParseTextAlign(
        p->last_h2_style.c_str(), &align);
    if (!have_align) {
      have_align = epub_css_class_map::LookupTextAlignForClassAttr(
          p->last_h2_class, p->css_class_map, &align);
    }
  } else if (parse_in(p, TAG_H3) || parse_in(p, TAG_H4) || parse_in(p, TAG_H5) ||
             parse_in(p, TAG_H6)) {
    in_alignable_block = true;
    have_align = book_xml_css_style_utils::TryParseTextAlign(
        p->last_h_style.c_str(), &align);
    if (!have_align) {
      have_align = epub_css_class_map::LookupTextAlignForClassAttr(
          p->last_h_class, p->css_class_map, &align);
    }
  }

  if (!in_alignable_block)
    return;

  if (!have_align || align == book_xml_css_style_utils::TextAlign::Left) {
    parse_append_page_byte(p, TEXT_PARAGRAPH_LEFT);
  } else if (align == book_xml_css_style_utils::TextAlign::Center) {
    parse_append_page_byte(p, TEXT_PARAGRAPH_CENTER);
  } else if (align == book_xml_css_style_utils::TextAlign::Right) {
    parse_append_page_byte(p, TEXT_PARAGRAPH_RIGHT);
  } else {
    parse_append_page_byte(p, TEXT_PARAGRAPH_LEFT);
  }
}

inline void RestoreParsedStyleMarkers(parsedata_t *p) {
  if (!p)
    return;
  RestoreParsedParagraphAlignmentMarker(p);
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

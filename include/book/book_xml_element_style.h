/*
    3dslibris - book_xml_element_style.h

    Per-element CSS/style attribute parsing and block layout configuration.
    Extracted from book_xml_parser.cpp.

    Covers: ParseElementClear/Float/TextTransform/WhiteSpace,
    ApplyElementBlockMargins, ParseElementMarginTopWithClass,
    ConfigureBlockTextAlign, EnsureBlockBoundaryBeforeBlockStart.
*/

#pragma once

#include "book/book_xml_css_style_utils.h"
#include "book/epub_css_class_map.h"
#include "parse.h"

struct parsedata_t;
class Text;

namespace book_xml_element_style {

book_xml_css_style_utils::ClearMode ParseElementClear(
    const char **attr, const epub_css_class_map::CssClassMargins &elem_css);

book_xml_css_style_utils::FloatMode ParseElementFloat(
    const char **attr, const epub_css_class_map::CssClassMargins &elem_css);

// Returns (u8)TextTransform value, or 0 if none.
u8 ParseElementTextTransform(const char **attr,
                              const epub_css_class_map::CssClassMargins &elem_css);

// Returns (u8)WhiteSpaceMode + 1, or 0 if none (matches parsedata_t stack encoding).
u8 ParseElementWhiteSpace(const char **attr,
                           const epub_css_class_map::CssClassMargins &elem_css);

void ApplyElementBlockMargins(parsedata_t *p, Text *ts, const char **attr,
                               const epub_css_class_map::CssClassMargins &elem_css);

book_xml_css_style_utils::MarginTopResult ParseElementMarginTopWithClass(
    const char **attr, const epub_css_class_map::CssClassMargins &elem_css);

void ConfigureBlockTextAlign(parsedata_t *p, const char *el, const char **attr,
                              const epub_css_class_map::CssClassMargins &elem_css);

// Force a mandatory block boundary before a block element that may be nested
// inside an inline styling wrapper. Emits '\n' immediately (not via pending
// model) so the break lands in the buffer before any style tokens the block
// handler emits. No-op if current line is already fresh.
void EnsureBlockBoundaryBeforeBlockStart(parsedata_t *p, const char *tag,
                                          const char *reason);

} // namespace book_xml_element_style

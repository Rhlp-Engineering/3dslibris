/*
    3dslibris - book_xml_image_handler.h

    Inline image handler: processes <img>/<image> start events during XML
    parsing, resolves image paths, plans layout, and emits image tokens into
    the page buffer.
*/

#pragma once

#include "book/epub_css_class_map.h"

struct parsedata_t;
class Text;

// Callbacks into book_xml_parser.cpp for functions that live inside its
// anonymous namespace. Populated by the parser and passed to HandleInlineImageStart.
struct ImageHandlerFns {
  void (*linefeed)(parsedata_t *p);
  void (*advance_screen)(parsedata_t *p);
  void (*advance_page_overflow)(parsedata_t *p, int lineheight);
  void (*emit_chardata)(parsedata_t *p, const char *txt, int len);
};

// Handle an <img> or <image> start event. Pushes TAG_UNKNOWN onto the stack.
void HandleInlineImageStart(parsedata_t *p, Text *ts, const char **attr,
                            const epub_css_class_map::CssClassMargins &elem_css,
                            const ImageHandlerFns &fns);

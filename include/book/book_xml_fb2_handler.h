/*
    3dslibris - book_xml_fb2_handler.h

    FB2-specific element handling extracted from book_xml_parser.cpp.

    Covers four sites in the parser:
      start()    — section depth + title-capture tracking; <binary> element setup
      chardata() — binary data collection; <title> text capture
      end()      — <binary> finalization; <title>/<section> end tracking
*/

#pragma once

#include "parse.h"

struct parsedata_t;

namespace book_xml_fb2_handler {

// Call from start() before the element dispatch.
// Updates fb2_section_depth and fb2_title_capture_depth when inside TAG_BODY.
void HandleFb2SectionStart(parsedata_t *p, const char *el);

// Call from start() as an else-if in the element dispatch.
// Returns true if el is "binary" (pushes TAG_UNKNOWN and sets up collection).
bool HandleFb2BinaryStart(parsedata_t *p, const char *el, const char **attr);

// Call from chardata() before the text emission pipeline.
// Returns true if the data was consumed — caller must return immediately.
// Handles: collecting_fb2_binary path and TAG_TITLE path (both FB2 and EPUB).
bool HandleFb2Chardata(parsedata_t *p, const char *txt, int txtlen);

// Call from end() before the element chain.
// Returns true if el is "binary" (stores image, resets state, calls parse_pop).
// Caller must return immediately when this returns true.
bool HandleFb2BinaryEnd(parsedata_t *p, const char *el);

// Call from end() after HandleFb2BinaryEnd returns false.
// Handles title finalization and section depth decrement in fb2_mode.
// Does NOT call parse_pop.
void HandleFb2TitleSectionEnd(parsedata_t *p, const char *el);

} // namespace book_xml_fb2_handler

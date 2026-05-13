/*
    3dslibris - book_xml_anchor_handler.h

    Anchor/link handler: processes <a> start and end events during XML parsing.
    Registers href targets with the book and emits TEXT_LINK_START/END tokens.
*/

#pragma once

struct parsedata_t;

// Callbacks into book_xml_parser.cpp for anonymous-namespace functions.
struct AnchorHandlerFns {
  void (*linefeed)(parsedata_t *p);
};

// Handle <a> start. Always pushes TAG_ANCHOR; emits TEXT_LINK_START if href
// resolves to a registered link.
void HandleAnchorStart(parsedata_t *p, const char **attr);

// Handle </a>. Emits TEXT_LINK_END if the link was active, and a linefeed
// on TOC-like documents.
void HandleAnchorEnd(parsedata_t *p, const AnchorHandlerFns &fns);

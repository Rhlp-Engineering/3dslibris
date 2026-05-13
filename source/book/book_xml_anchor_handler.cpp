/*
    3dslibris - book_xml_anchor_handler.cpp

    Anchor/link handler extracted from book_xml_parser.cpp.

    Handles <a>/<href> start/end events: resolves internal hrefs, registers
    them with the book, and emits TEXT_LINK_START/END tokens into the page buffer.
*/

#include "book/book_xml_anchor_handler.h"
#include "book/book.h"
#include "book/book_xml_inline_state.h"
#include "parse.h"
#include "reader/inline_link_utils.h"

#include <cctype>
#include <string.h>
#include <string>

namespace {

static bool EqualsAsciiNoCaseLocal(const char *a, const char *b) {
  if (!a || !b)
    return false;
  while (*a && *b) {
    unsigned char ca = (unsigned char)*a, cb = (unsigned char)*b;
    if (ca >= 'A' && ca <= 'Z') ca = (unsigned char)(ca - 'A' + 'a');
    if (cb >= 'A' && cb <= 'Z') cb = (unsigned char)(cb - 'A' + 'a');
    if (ca != cb) return false;
    a++; b++;
  }
  return *a == '\0' && *b == '\0';
}

static bool XmlNameEqualsLocal(const char *name, const char *needle) {
  if (!name || !needle)
    return false;
  if (!strcmp(name, needle) || EqualsAsciiNoCaseLocal(name, needle))
    return true;
  const char *colon = strrchr(name, ':');
  return colon && (strcmp(colon + 1, needle) == 0 ||
                   EqualsAsciiNoCaseLocal(colon + 1, needle));
}

static bool PathLooksLikeTocDoc(const std::string &path) {
  if (path.empty())
    return false;
  std::string lower = path;
  for (size_t i = 0; i < lower.size(); i++)
    lower[i] = (char)tolower((unsigned char)lower[i]);
  return lower.find("toc") != std::string::npos ||
         lower.find("indice") != std::string::npos ||
         lower.find("index") != std::string::npos ||
         lower.find("contents") != std::string::npos ||
         lower.find("contenido") != std::string::npos ||
         lower.find("nav") != std::string::npos;
}

static bool DocLooksLikeTocDoc(const parsedata_t *p) {
  if (!p)
    return false;
  return PathLooksLikeTocDoc(p->docpath) ||
         PathLooksLikeTocDoc(p->doc_title) ||
         PathLooksLikeTocDoc(p->doc_heading);
}

} // namespace

void HandleAnchorStart(parsedata_t *p, const char **attr) {
  parse_push(p, TAG_ANCHOR);
  const u8 current = (u8)(p->stacksize - 1);
  p->link_active_stack[current] = false;
  p->link_href_id_stack[current] = 0;
  if (book_xml_inline_state::HasActiveStackHiddenStyle(p))
    return;
  const char *href = NULL;
  for (int i = 0; attr && attr[i]; i += 2) {
    if (XmlNameEqualsLocal(attr[i], "href")) {
      href = attr[i + 1];
      break;
    }
  }
  const std::string resolved_href =
      (href && *href)
          ? inline_link_utils::ResolveInternalHref(p->docpath, href)
          : std::string();
  if (!resolved_href.empty() && p->book) {
    const u16 href_id = p->book->RegisterInlineLinkHref(resolved_href);
    if (href_id != 0) {
      p->link_active_stack[current] = true;
      p->link_href_id_stack[current] = href_id;
      parse_append_page_byte(p, TEXT_LINK_START);
      parse_append_page_byte(p, (u32)href_id);
    }
  }
}

void HandleAnchorEnd(parsedata_t *p, const AnchorHandlerFns &fns) {
  if (p->stacksize > 0) {
    const u8 current = (u8)(p->stacksize - 1);
    if (p->link_active_stack[current] && p->link_href_id_stack[current] != 0)
      parse_append_page_byte(p, TEXT_LINK_END);
  }
  // TOC/Nav documents are dense anchor lists; force line breaks for readability.
  if (DocLooksLikeTocDoc(p) && p->linebegan && p->buflen > 0 &&
      p->buf[p->buflen - 1] != '\n') {
    fns.linefeed(p);
  }
}

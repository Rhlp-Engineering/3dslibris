/*
 * EPUB parser stubs for tests that exercise book_parser.cpp dispatch without
 * linking the real EPUB implementation.
 */
#include "formats/epub/epub_parser.h"

namespace epub_parser {
uint8_t Open(Book *, const std::string &) { return 1; }
uint8_t Index(Book *, const std::string &) { return 1; }
int ExtractCover(Book *, const std::string &) { return -1; }
int ResolveToc(Book *, const std::string &) { return -1; }
} // namespace epub_parser

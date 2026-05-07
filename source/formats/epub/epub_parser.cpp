#include "formats/epub/epub_parser.h"
#include "formats/epub/epub.h"

namespace epub_parser {

uint8_t Open(Book *book, const std::string &path) {
  if (!book || path.empty())
    return 1;
  return (uint8_t)epub(book, path, false);
}

uint8_t Index(Book *book, const std::string &path) {
  if (!book || path.empty())
    return 1;
  return (uint8_t)epub(book, path, true);
}

int ExtractCover(Book *book, const std::string &path) {
  return epub_extract_cover(book, path);
}

int ResolveToc(Book *book, const std::string &path) {
  return epub_resolve_toc(book, path);
}

} // namespace epub_parser

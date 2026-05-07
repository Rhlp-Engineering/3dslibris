#include "formats/mobi/mobi_parser.h"
#include "formats/mobi/mobi_book_hooks.h"
#include "formats/mobi/mobi.h"

namespace mobi_parser {

u8 Parse(Book *book, const char *path) {
  static const Hooks hooks = mobi_book_hooks::Make();
  return ParseFile(book, path, hooks);
}

int ExtractCover(Book *book, const std::string &path) {
  return mobi_extract_cover(book, path);
}

} // namespace mobi_parser

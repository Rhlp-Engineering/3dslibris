#include "formats/mobi/mobi_parser.h"
#include "formats/mobi/mobi_book_hooks.h"

namespace mobi_parser {

u8 Parse(Book *book, const char *path) {
  static const Hooks hooks = mobi_book_hooks::Make();
  return ParseFile(book, path, hooks);
}

} // namespace mobi_parser

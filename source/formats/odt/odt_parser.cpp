#include "formats/odt/odt_parser.h"

#include "formats/odt/odt_loader.h"

namespace odt_parser {

uint8_t Parse(Book *book, const char *path) {
  return odt_loader::ParseOdtFile(book, path);
}

} // namespace odt_parser

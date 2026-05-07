#include "formats/cbz/cbz_parser.h"
#include "formats/cbz/cbz.h"

namespace cbz_parser {

uint8_t Parse(Book *book, const char *path) {
  return ParseCbzFile(book, path);
}

uint8_t Index(Book *book, const char *path) {
  return IndexCbzMetadata(book, path);
}

int ExtractCover(Book *book, const std::string &path) {
  return cbz_extract_cover(book, path);
}

} // namespace cbz_parser

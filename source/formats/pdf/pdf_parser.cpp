#include "formats/pdf/pdf_parser.h"
#include "formats/pdf/pdf.h"

namespace pdf_parser {

uint8_t Parse(Book *book, const char *path) {
  return ParsePdfFile(book, path);
}

uint8_t Index(Book *book, const char *path) {
  return IndexPdfMetadata(book, path);
}

int ExtractCover(Book *book, const std::string &path) {
  return pdf_extract_cover(book, path);
}

} // namespace pdf_parser

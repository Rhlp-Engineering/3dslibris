#include "formats/txt/txt_parser.h"

#include "formats/common/book_error.h"
#include "formats/common/plain_parser.h"
#include "formats/txt/txt_loader.h"

#include <string>

namespace txt_parser {

uint8_t Parse(Book *book, const char *path) {
  std::string text;
  if (!txt_loader::ReadAndNormalize(path, &text))
    return BOOK_ERR_CORRUPT;
  return plain_parser::ParseBuffer(book, text);
}

} // namespace txt_parser

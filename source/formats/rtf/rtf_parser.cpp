#include "formats/rtf/rtf_parser.h"

#include "formats/common/book_error.h"
#include "formats/common/plain_parser.h"
#include "formats/rtf/rtf_loader.h"

#include <string>

namespace rtf_parser {

uint8_t Parse(Book *book, const char *path) {
  std::string text;
  if (!rtf_loader::ReadAndDecode(path, &text))
    return BOOK_ERR_CORRUPT;
  return plain_parser::ParseBuffer(book, text);
}

} // namespace rtf_parser

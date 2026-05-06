#pragma once

#include "book/book_parse_deps.h"

#include <stdint.h>
#include <string>

class Book;

namespace fb2_parser {

uint8_t Parse(Book *book, const char *path, bool fulltext,
              const BookParseDeps &deps);
int ExtractCover(Book *book, const std::string &path);

} // namespace fb2_parser

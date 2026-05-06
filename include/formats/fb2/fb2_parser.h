#pragma once

#include "book/book_parse_deps.h"

#include <stdint.h>

class Book;

namespace fb2_parser {

uint8_t Parse(Book *book, const char *path, bool fulltext,
              const BookParseDeps &deps);

} // namespace fb2_parser

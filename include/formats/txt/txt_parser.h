#pragma once

#include <stdint.h>

class Book;

namespace txt_parser {

uint8_t Parse(Book *book, const char *path);

} // namespace txt_parser

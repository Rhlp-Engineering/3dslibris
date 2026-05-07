#pragma once

#include <stdint.h>

class Book;

namespace odt_parser {

uint8_t Parse(Book *book, const char *path);

} // namespace odt_parser

#pragma once

#include <stdint.h>

class Book;

namespace rtf_parser {

uint8_t Parse(Book *book, const char *path);

} // namespace rtf_parser

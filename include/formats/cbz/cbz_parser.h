#pragma once

#include <stdint.h>
#include <string>

class Book;

namespace cbz_parser {

uint8_t Parse(Book *book, const char *path);
uint8_t Index(Book *book, const char *path);
int ExtractCover(Book *book, const std::string &path);

} // namespace cbz_parser

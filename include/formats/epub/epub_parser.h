#pragma once

#include <stdint.h>
#include <string>

class Book;

namespace epub_parser {

uint8_t Open(Book *book, const std::string &path);
uint8_t Index(Book *book, const std::string &path);
int ExtractCover(Book *book, const std::string &path);
int ResolveToc(Book *book, const std::string &path);

} // namespace epub_parser

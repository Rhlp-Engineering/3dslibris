/*
    3dslibris - odt_loader.h

    ODT parser entry point extracted from book_io.cpp.
*/

#pragma once

#include "book/book.h"

namespace odt_loader {

u8 ParseOdtFile(Book *book, const char *path);

} // namespace odt_loader


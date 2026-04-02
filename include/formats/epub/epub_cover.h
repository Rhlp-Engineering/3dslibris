/*
    3dslibris - epub_cover.h
    EPUB cover image extraction and decoding.
    Extracted from epub.cpp by Rigle.
*/

#pragma once

#include <string>

class Book;

namespace epub_cover {

int Extract(Book *book, const std::string &epubpath);

} // namespace epub_cover

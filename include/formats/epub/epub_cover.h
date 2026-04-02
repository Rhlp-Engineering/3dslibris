/*
    3dslibris - epub_cover.h
    EPUB cover image extraction and decoding.
    Extracted from epub.cpp by Rigle.
*/

#pragma once

#include "formats/epub/epub.h"
#include <string>

class Book;

namespace epub_cover {

int Extract(Book *book, const std::string &epubpath);

bool FindLikelyImagePath(epub_data_t &data, const std::string &opf_folder,
                         std::string &path_out);

} // namespace epub_cover

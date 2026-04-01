/*
    3dslibris - epub_page_cache.h
    EPUB persistent page cache serialization.
    Extracted from epub.cpp by Rigle.
*/

#pragma once

#include <string>

class Book;

namespace epub_page_cache {

bool TryLoad(Book *book, const char *book_path, int pixel_size,
             int line_spacing, int paragraph_spacing, int paragraph_indent,
             int orientation, int margin_left, int margin_right, int margin_top,
             int margin_bottom, const char *regular_font);

void Save(Book *book, const char *book_path, int pixel_size,
          int line_spacing, int paragraph_spacing, int paragraph_indent,
          int orientation, int margin_left, int margin_right, int margin_top,
          int margin_bottom, const char *regular_font);

void SavePending(Book *book);

} // namespace epub_page_cache

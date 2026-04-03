#pragma once

#include "book/book_parse_deps.h"

class Book;

namespace xml_book_parser {

typedef void (*BuildFb2FallbackChaptersFn)(Book *book);
typedef void (*SetTocConfidenceFn)(Book *book, bool strong);

u8 ParseXmlBookFile(Book *book, const char *path, bool fulltext,
                    const BookParseDeps &deps,
                    BuildFb2FallbackChaptersFn build_fb2_fallback,
                    SetTocConfidenceFn set_toc_confidence);

} // namespace xml_book_parser

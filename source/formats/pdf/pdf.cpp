// SPDX-License-Identifier: AGPL-3.0-or-later

#include "formats/pdf/pdf.h"

#include "formats/mupdf/mupdf_document.h"

uint8_t ParsePdfFile(Book *book, const char *path) {
  return ParseMuPdfFile(book, path);
}

uint8_t IndexPdfMetadata(Book *book, const char *path) {
  return IndexMuPdfMetadata(book, path);
}

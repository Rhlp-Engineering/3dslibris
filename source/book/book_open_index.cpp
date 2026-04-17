/*
    3dslibris - book_open_index.cpp
*/

#include "book/book.h"

#include "formats/cbz/cbz.h"
#include "formats/common/book_error.h"
#include "formats/epub/epub.h"
#include "formats/pdf/pdf.h"

u8 Book::Open() {
  PrepareForOpen();
  return OpenPrepared();
}

u8 Book::Index() {
  if (metadataIndexTried)
    return metadataIndexed ? 0 : 1;
  metadataIndexTried = true;

  int err = 1;
  if (format == FORMAT_EPUB) {
    std::string path;
    path.append(GetFolderName());
    path.append("/");
    path.append(GetFileName());
    err = epub(this, path, true);
  } else if (format == FORMAT_PDF) {
    std::string path;
    path.append(GetFolderName());
    path.append("/");
    path.append(GetFileName());
    err = IndexPdfMetadata(this, path.c_str());
  } else if (format == FORMAT_CBZ) {
    std::string path;
    path.append(GetFolderName());
    path.append("/");
    path.append(GetFileName());
    err = IndexCbzMetadata(this, path.c_str());
  } else {
    err = 0;
  }
  if (err == BOOK_ERR_CANCELLED) {
    metadataIndexTried = false;
    metadataIndexed = false;
    return err;
  }
  if (!err) {
    metadataIndexed = true;
  }
  return err;
}

/*
    3dslibris - book_open_index.cpp
*/

#include "book/book.h"

#include <sys/stat.h>

#include "formats/cbz/cbz.h"
#include "formats/common/book_error.h"
#include "formats/common/book_meta_cache.h"
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

  std::string path;
  path.append(GetFolderName());
  path.append("/");
  path.append(GetFileName());

  // Check the disk cache before opening the source file.
  book_meta_cache::EnsureDir();
  struct stat st;
  long long fsize = 0, fmtime = 0;
  if (stat(path.c_str(), &st) == 0) {
    fsize  = (long long)st.st_size;
    fmtime = (long long)st.st_mtime;
  }
  const std::string cache_path =
      book_meta_cache::BuildPath(path, fsize, fmtime);
  book_meta_cache::MetaEntry cached;
  if (book_meta_cache::Load(cache_path, &cached)) {
    if (!cached.title.empty())
      SetTitle(cached.title.c_str());
    if (!cached.author.empty())
      SetAuthor(cached.author);
    coverImagePath = cached.cover_image_path;
    ClearBrowserDisplayNameCache();
    metadataIndexed = true;
    return 0;
  }

  int err = 1;
  if (format == FORMAT_EPUB) {
    err = epub(this, path, true);
  } else if (format == FORMAT_PDF) {
    err = IndexPdfMetadata(this, path.c_str());
  } else if (format == FORMAT_CBZ) {
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
    book_meta_cache::MetaEntry entry;
    entry.title            = title;
    entry.author           = author;
    entry.cover_image_path = coverImagePath;
    book_meta_cache::Save(cache_path, entry);
  }

  return err;
}

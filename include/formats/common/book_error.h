#pragma once

#include <stdint.h>

static const uint8_t BOOK_ERR_PASSWORD = 248;
static const uint8_t BOOK_ERR_CORRUPT = 249;
static const uint8_t BOOK_ERR_CANCELLED = 250;

// Short tag for debug/timing logs where the full user-facing text is too noisy.
inline const char *BookOpenErrorTag(int err) {
  switch (err) {
  case BOOK_ERR_PASSWORD:
    return "password_protected_book";
  case BOOK_ERR_CORRUPT:
    return "corrupt_or_empty_book";
  case BOOK_ERR_CANCELLED:
    return "cancelled";
  default:
    return 0;
  }
}

// Human-readable message used when opening a broken or empty source file.
inline const char *DescribeBookOpenError(int err) {
  switch (err) {
  case BOOK_ERR_PASSWORD:
    return "error: password-protected or encrypted book";
  case BOOK_ERR_CORRUPT:
    return "error: corrupt or empty book";
  case BOOK_ERR_CANCELLED:
    return "loading cancelled";
  default:
    return 0;
  }
}

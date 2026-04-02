/*
    3dslibris - epub_limits.h
    EPUB format constants and size limits.
    Extracted from epub.cpp by Rigle.
*/

#pragma once

#include <cstddef>

namespace epub_limits {

static const size_t kTocMaxBytes = 192 * 1024;
static const size_t kTocMaxEntries = 2048;
static const size_t kCoverMaxEntryBytes = 8 * 1024 * 1024;
static const size_t kCoverMaxNonJpegBytes = 2 * 1024 * 1024;
static const size_t kCoverMaxDecodedRgbBytes = 16 * 1024 * 1024;
static const size_t kSvgWrapperMaxBytes = 512 * 1024;
static const int kCoverMaxDimension = 4096;

static const bool kEnableRealTocResolve = false;

} // namespace epub_limits

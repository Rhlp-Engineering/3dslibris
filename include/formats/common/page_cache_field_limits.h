/*
    3dslibris - page_cache_field_limits.h
    Shared field size caps used by all format-specific page caches.
*/

#pragma once

#include <stdint.h>

namespace page_cache_limits {

static const uint16_t kTitleMaxBytes        = 1000;
static const uint16_t kPageMaxBytes         = 4096;
static const uint16_t kChapterTitleMaxBytes = 2048;
static const uint16_t kPathMaxBytes         = 2048;

} // namespace page_cache_limits

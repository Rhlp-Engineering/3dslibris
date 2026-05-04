#pragma once

#include <vector>

namespace fixed_layout_bitmap_cache {

struct Base {
  int page;
  int zoom_index;
  int bitmap_width;
  int bitmap_height;
  std::vector<u16> pixels;

  Base()
      : page(-1), zoom_index(-1),
        bitmap_width(0), bitmap_height(0),
        pixels() {}
};

} // namespace fixed_layout_bitmap_cache

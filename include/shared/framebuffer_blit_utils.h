#pragma once

#include <cstddef>

namespace framebuffer_blit_utils {

inline int LogicalTextScreenHeight(bool is_left_screen) {
  return is_left_screen ? 400 : 320;
}

inline size_t LogicalTextScreenPixelCount(int logical_width,
                                          bool is_left_screen) {
  if (logical_width <= 0)
    return 0;
  return (size_t)logical_width *
         (size_t)LogicalTextScreenHeight(is_left_screen);
}

} // namespace framebuffer_blit_utils

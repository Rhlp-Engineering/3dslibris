#pragma once

namespace frame_debug_utils {

inline bool ShouldLogBlitPage(bool dirty, bool needs_copy) {
  return dirty || needs_copy;
}

} // namespace frame_debug_utils

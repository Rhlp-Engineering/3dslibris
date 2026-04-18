#pragma once

namespace fixed_layout_viewport_utils {

enum PageTurnDirection {
  PAGE_TURN_LEFT_TO_RIGHT = 0,
  PAGE_TURN_RIGHT_TO_LEFT = 1,
};

struct ViewportCenter {
  float x;
  float y;
};

inline ViewportCenter DefaultPageTurnViewportCenter(
    PageTurnDirection direction = PAGE_TURN_LEFT_TO_RIGHT) {
  ViewportCenter center = {0.0f, 0.0f};
  if (direction == PAGE_TURN_RIGHT_TO_LEFT)
    center.x = 1.0f;
  return center;
}

} // namespace fixed_layout_viewport_utils

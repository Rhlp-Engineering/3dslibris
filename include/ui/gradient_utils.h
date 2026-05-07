#pragma once

#include <3ds.h>

class Text;

namespace gradient_utils {

void DrawToScreen(Text *ts, int color_mode, u16 *target_screen, int logical_h);

} // namespace gradient_utils

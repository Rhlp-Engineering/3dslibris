#pragma once

struct InlineImageScreenLayout {
  int current_screen_height;
  int current_margin_bottom;
  int next_screen_height;
  int next_margin_bottom;
};

InlineImageScreenLayout ResolveInlineImageScreenLayout(
    bool current_screen_is_left, int full_bottom_margin);

InlineImageScreenLayout ResolveInlineImageScreenLayoutForReadingScreen(
    bool turned_right, int current_screen, int full_bottom_margin);

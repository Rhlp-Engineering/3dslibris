#include "formats/common/fixed_layout_blit_utils.h"

#include "ui/text.h"

#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

namespace {

[[noreturn]] void Fail(const std::string &message) {
  fprintf(stderr, "%s\n", message.c_str());
  std::exit(1);
}

void ExpectEq(const char *label, u16 actual, u16 expected) {
  if (actual != expected)
    Fail(std::string(label) + ": expected equality");
}

u16 SourcePixel(int x, int y) {
  return (u16)(0x1000 + y * 0x0100 + x);
}

void TestExactCopyClipsNegativeOriginWithCropOffset() {
  Text ts;
  ts.display.width = 5;
  ts.display.height = 6;

  std::vector<u16> screen((size_t)ts.display.height * 4, 0xEEEE);
  std::vector<u16> pixels((size_t)6 * 5, 0);
  for (int y = 0; y < 5; y++) {
    for (int x = 0; x < 6; x++)
      pixels[(size_t)y * 6 + x] = SourcePixel(x, y);
  }

  fixed_layout_blit_utils::BlitRgb565BitmapScaledCrop(
      &ts, screen.data(), 4, -1, -1, 4, 3, pixels, 6, 5, 1, 1, 4, 3, false);

  ExpectEq("left clipped row0 col0", screen[0 * 6 + 0], SourcePixel(2, 2));
  ExpectEq("left clipped row0 col1", screen[0 * 6 + 1], SourcePixel(3, 2));
  ExpectEq("left clipped row0 col2", screen[0 * 6 + 2], SourcePixel(4, 2));
  ExpectEq("left clipped row0 col3 untouched", screen[0 * 6 + 3], 0xEEEE);
  ExpectEq("left clipped row1 col0", screen[1 * 6 + 0], SourcePixel(2, 3));
  ExpectEq("left clipped row1 col2", screen[1 * 6 + 2], SourcePixel(4, 3));
  ExpectEq("unused stride col untouched", screen[0 * 6 + 5], 0xEEEE);
}

void TestExactCopyClipsRightEdgeWithCropOffset() {
  Text ts;
  ts.display.width = 5;
  ts.display.height = 6;

  std::vector<u16> screen((size_t)ts.display.height * 4, 0xEEEE);
  std::vector<u16> pixels((size_t)6 * 5, 0);
  for (int y = 0; y < 5; y++) {
    for (int x = 0; x < 6; x++)
      pixels[(size_t)y * 6 + x] = SourcePixel(x, y);
  }

  fixed_layout_blit_utils::BlitRgb565BitmapScaledCrop(
      &ts, screen.data(), 4, 3, 1, 4, 2, pixels, 6, 5, 1, 2, 4, 2, false);

  ExpectEq("right clipped row0 col3", screen[1 * 6 + 3], SourcePixel(1, 2));
  ExpectEq("right clipped row0 col4", screen[1 * 6 + 4], SourcePixel(2, 2));
  ExpectEq("right clipped row0 stride untouched", screen[1 * 6 + 5], 0xEEEE);
  ExpectEq("right clipped row1 col3", screen[2 * 6 + 3], SourcePixel(1, 3));
  ExpectEq("right clipped row1 col4", screen[2 * 6 + 4], SourcePixel(2, 3));
}

} // namespace

int main() {
  TestExactCopyClipsNegativeOriginWithCropOffset();
  TestExactCopyClipsRightEdgeWithCropOffset();
  return 0;
}

#include <cstdio>
#include <cstdlib>
#include <string>

namespace {

[[noreturn]] void Fail(const std::string &message) {
  std::fprintf(stderr, "%s\n", message.c_str());
  std::exit(1);
}

void ExpectTrue(const char *label, bool value) {
  if (!value)
    Fail(std::string(label) + ": expected true");
}

bool ShouldSkipRawSvgCover(bool is_svg_cover,
                           bool resolved_to_embedded_raster) {
  return is_svg_cover && !resolved_to_embedded_raster;
}

} // namespace

int main() {
  ExpectTrue("raw svg stops before raster fallback",
             ShouldSkipRawSvgCover(true, false));
  ExpectTrue("resolved raster inside svg may continue",
             !ShouldSkipRawSvgCover(true, true));
  ExpectTrue("non-svg may continue",
             !ShouldSkipRawSvgCover(false, false));
  return 0;
}

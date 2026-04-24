#include "ui/frame_debug_utils.h"

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

void ExpectFalse(const char *label, bool value) {
  if (value)
    Fail(std::string(label) + ": expected false");
}

} // namespace

int main() {
  ExpectTrue("dirty page is worth logging",
             frame_debug_utils::ShouldLogBlitPage(true, false));
  ExpectTrue("framebuffer copy is worth logging",
             frame_debug_utils::ShouldLogBlitPage(false, true));
  ExpectFalse("steady idle frame is not logged",
              frame_debug_utils::ShouldLogBlitPage(false, false));
  return 0;
}

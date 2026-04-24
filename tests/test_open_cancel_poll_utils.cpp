#include "shared/open_cancel_poll_utils.h"

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
  ExpectFalse("no abort flags keep work running",
              open_cancel_poll_utils::ShouldCancel(false, false));
  ExpectTrue("reporter abort cancels work",
             open_cancel_poll_utils::ShouldCancel(true, false));
  ExpectTrue("book abort cancels work",
             open_cancel_poll_utils::ShouldCancel(false, true));
  ExpectTrue("either abort flag cancels work",
             open_cancel_poll_utils::ShouldCancel(true, true));
  return 0;
}

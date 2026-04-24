#include "reader/suspend_policy_utils.h"

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
  ExpectFalse("no opening book has nothing to keep",
              reader_suspend_policy_utils::ShouldKeepOpeningDuringSuspend(
                  false, false));
  ExpectFalse("synchronous opening should be cancelled on suspend",
              reader_suspend_policy_utils::ShouldKeepOpeningDuringSuspend(
                  true, false));
  ExpectTrue("async opening survives HOME suspend",
             reader_suspend_policy_utils::ShouldKeepOpeningDuringSuspend(
                 true, true));
  return 0;
}

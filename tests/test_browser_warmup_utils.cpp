#include "shared/browser_warmup_utils.h"

#include <cstdio>
#include <cstdlib>
#include <string>

namespace {

[[noreturn]] void Fail(const std::string &message) {
  fprintf(stderr, "%s\n", message.c_str());
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

void TestWarmupBlockedDuringReleaseWait() {
  ExpectFalse("release wait blocks warmup",
              browser_warmup_utils::IsBrowserWarmupIdle(1000, 0, true));
}

void TestWarmupRequiresIdleDelay() {
  ExpectFalse("below idle threshold",
              browser_warmup_utils::IsBrowserWarmupIdle(
                  browser_warmup_utils::kBrowserWarmupIdleDelayMs - 1, 0,
                  false));
  ExpectTrue("at idle threshold",
             browser_warmup_utils::IsBrowserWarmupIdle(
                 browser_warmup_utils::kBrowserWarmupIdleDelayMs, 0, false));
}

void TestWarmupRejectsWrappedClock() {
  ExpectFalse("wrapped clock rejected",
              browser_warmup_utils::IsBrowserWarmupIdle(50, 100, false));
}

} // namespace

int main() {
  TestWarmupBlockedDuringReleaseWait();
  TestWarmupRequiresIdleDelay();
  TestWarmupRejectsWrappedClock();
  return 0;
}

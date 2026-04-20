#include "shared/debug_runtime_mode.h"

#include <cstdio>
#include <cstdlib>
#include <string>

// This test documents the current state of each runtime flag.
// Update the expected value here whenever a flag is changed.

namespace {

struct FlagExpectation {
  const char *label;
  bool actual;
  bool expected;
};

} // namespace

int main() {
  FlagExpectation flags[] = {
      {"BackgroundWorkersDisabled", debug_runtime::BackgroundWorkersDisabled(),
       true},
      {"BrowserWarmupDisabled", debug_runtime::BrowserWarmupDisabled(), false},
      {"ForceSynchronousBookOpen", debug_runtime::ForceSynchronousBookOpen(),
       true},
      {"ForceSynchronousCbzDecode", debug_runtime::ForceSynchronousCbzDecode(),
       true},
      {"ForceSynchronousMuPdfRender",
       debug_runtime::ForceSynchronousMuPdfRender(), true},
      {"ForceSynchronousMobiFinalize",
       debug_runtime::ForceSynchronousMobiFinalize(), true},
  };

  bool ok = true;
  for (size_t i = 0; i < sizeof(flags) / sizeof(flags[0]); i++) {
    if (flags[i].actual != flags[i].expected) {
      std::fprintf(stderr, "%s: expected %s, got %s\n", flags[i].label,
                   flags[i].expected ? "true" : "false",
                   flags[i].actual ? "true" : "false");
      ok = false;
    }
  }
  if (!ok)
    return 1;

  std::fprintf(stdout, "All runtime mode flags match expected values.\n");
  return 0;
}

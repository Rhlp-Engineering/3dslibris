#include "reader/reflow_open_gate_utils.h"

#include <cstdio>
#include <cstdlib>
#include <string>

namespace {

[[noreturn]] void Fail(const std::string &msg) {
  std::fprintf(stderr, "%s\n", msg.c_str());
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

void TestNonTextLayoutNeverUsesWorker() {
  ExpectFalse("fixed layout disables worker",
              reader::ShouldUseAsyncReflowOpen(false));
}

void TestTextLayoutUsesWorker() {
  ExpectTrue("reflowable keeps worker",
             reader::ShouldUseAsyncReflowOpen(true));
}

} // namespace

int main() {
  TestNonTextLayoutNeverUsesWorker();
  TestTextLayoutUsesWorker();
  return 0;
}

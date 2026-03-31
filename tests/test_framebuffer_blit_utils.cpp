#include "shared/framebuffer_blit_utils.h"

#include <cstdio>
#include <cstdlib>
#include <string>

namespace {

[[noreturn]] void Fail(const std::string &message) {
  fprintf(stderr, "%s\n", message.c_str());
  std::exit(1);
}

void ExpectEq(const char *label, int actual, int expected) {
  if (actual != expected) {
    Fail(std::string(label) + ": expected " + std::to_string(expected) +
         ", got " + std::to_string(actual));
  }
}

void ExpectEqSize(const char *label, size_t actual, size_t expected) {
  if (actual != expected) {
    Fail(std::string(label) + ": expected " + std::to_string(expected) +
         ", got " + std::to_string(actual));
  }
}

void TestLogicalHeights() {
  ExpectEq("left logical height",
           framebuffer_blit_utils::LogicalTextScreenHeight(true), 400);
  ExpectEq("right logical height",
           framebuffer_blit_utils::LogicalTextScreenHeight(false), 320);
}

void TestLogicalPixelCounts() {
  ExpectEqSize("left logical pixels",
               framebuffer_blit_utils::LogicalTextScreenPixelCount(240, true),
               (size_t)96000);
  ExpectEqSize("right logical pixels",
               framebuffer_blit_utils::LogicalTextScreenPixelCount(240, false),
               (size_t)76800);
}

} // namespace

int main() {
  TestLogicalHeights();
  TestLogicalPixelCounts();
  return 0;
}

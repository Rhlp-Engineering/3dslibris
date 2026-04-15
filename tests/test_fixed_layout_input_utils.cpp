#include "reader/fixed_layout_input_utils.h"

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
  ExpectTrue("pdf supports shoulder page turns",
             reader_input_utils::FixedLayoutSupportsShoulderPageTurn(
                 FORMAT_PDF));
  ExpectTrue("cbz supports shoulder page turns",
             reader_input_utils::FixedLayoutSupportsShoulderPageTurn(
                 FORMAT_CBZ));
  ExpectFalse("epub does not use fixed-layout shoulder mapping",
              reader_input_utils::FixedLayoutSupportsShoulderPageTurn(
                  FORMAT_EPUB));
  return 0;
}

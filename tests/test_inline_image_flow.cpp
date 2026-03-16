#include "inline_image_flow.h"

#include <cstdlib>
#include <string>

namespace {

[[noreturn]] void Fail(const std::string &message) {
  fprintf(stderr, "%s\n", message.c_str());
  std::exit(1);
}

void ExpectPlan(const char *label, InlineImageFlowPlan actual,
                bool advance_before, int next_text_screen, int page_breaks) {
  if (actual.advance_before != advance_before ||
      actual.next_text_screen != next_text_screen ||
      actual.page_breaks != page_breaks) {
    Fail(std::string(label) + ": unexpected inline image flow plan");
  }
}

} // namespace

int main() {
  ExpectPlan("fresh left screen keeps image on left",
             PlanInlineImageFlow(0, true), false, 1, 0);
  ExpectPlan("mid left screen moves image to right and text to next page",
             PlanInlineImageFlow(0, false), true, 0, 1);
  ExpectPlan("fresh right screen moves text to next page",
             PlanInlineImageFlow(1, true), false, 0, 1);
  ExpectPlan("mid right screen moves image to next page left",
             PlanInlineImageFlow(1, false), true, 1, 1);
  return 0;
}

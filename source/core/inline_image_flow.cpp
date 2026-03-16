#include "inline_image_flow.h"

InlineImageFlowPlan PlanInlineImageFlow(int current_screen,
                                        bool at_screen_start) {
  InlineImageFlowPlan plan;
  plan.advance_before = !at_screen_start;
  plan.next_text_screen = current_screen;
  plan.page_breaks = 0;

  int image_screen = current_screen;
  if (plan.advance_before) {
    if (image_screen == 1) {
      image_screen = 0;
      plan.page_breaks++;
    } else {
      image_screen = 1;
    }
  }

  if (image_screen == 1) {
    plan.next_text_screen = 0;
    plan.page_breaks++;
  } else {
    plan.next_text_screen = 1;
  }

  return plan;
}

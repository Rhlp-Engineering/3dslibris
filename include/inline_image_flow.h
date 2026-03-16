#pragma once

struct InlineImageFlowPlan {
  bool advance_before;
  int next_text_screen;
  int page_breaks;
};

InlineImageFlowPlan PlanInlineImageFlow(int current_screen,
                                        bool at_screen_start);

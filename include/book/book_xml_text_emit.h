#pragma once

#include "parse.h"
#include "shared/text_bidi_utils.h"
#include "shared/text_layout_utils.h"

#include <string>

namespace book_xml_text_emit {

struct FlowEmitMetrics {
  int display_width;
  int base_margin_left;
  int margin_left;
  int margin_right;
  int lineheight;
  int linespacing;
  int spaceadvance;
  bool text_already_transformed;
  // Visible threshold: max_height - bottom_margin for the current screen.
  // Used to guard the LTR advance check: a candidate line at pen.y is allowed
  // if pen.y <= overflow_threshold, even if pen.y + step > overflow_threshold.
  // When zero (e.g. in tests that don't populate it), the guard is disabled
  // and the old WouldOverflow behavior is preserved.
  int overflow_threshold;
  int screen_max_height;
  int screen_bottom_margin;
  // First-line text-indent in pixels. Non-zero only for the first chardata
  // call of a paragraph (paragraph_has_content == false at build time).
  // Applied to pen.x before the first word is emitted; wrap lines are not
  // indented (they reset pen.x to margin_left, not margin_left+text_indent).
  int text_indent_px;
};

typedef void (*AdvancePageOnOverflowFn)(parsedata_t *p, int lineheight,
                                        void *ctx);

bool ParsedBufferEndsWithWhitespace(const parsedata_t *p);
std::string TransformUtf8ForLayout(parsedata_t *p, const char *utf8,
                                   size_t utf8_len);
void AppendParsedCodepoints(parsedata_t *p, const char *utf8, size_t utf8_len);
void AppendParsedCodepointsRaw(parsedata_t *p, const char *utf8,
                               size_t utf8_len);
void EmitBidiSegment(parsedata_t *p,
                     const std::vector<text_layout_utils::ShapedGlyph> &run,
                     size_t seg_start, size_t seg_end,
                     const std::vector<text_bidi_utils::BidiRun> &runs,
                     bool apply_text_transform = true);
bool DetectParagraphRTL(
    const std::vector<text_layout_utils::ShapedGlyph> &run);

void EmitFlowedShapedText(
    parsedata_t *p, const char *txt,
    const std::vector<text_layout_utils::ShapedGlyph> &run, bool has_rtl,
    const std::vector<text_bidi_utils::BidiRun> &bidi_runs,
    const FlowEmitMetrics &metrics,
    AdvancePageOnOverflowFn advance_page_on_overflow, void *advance_ctx);

} // namespace book_xml_text_emit

/*
    3dslibris - plain_text_perf_utils.cpp

    Shared plain-text parse performance counters used by book_io paths.
*/

#include "formats/common/plain_text_perf_utils.h"

#include "shared/debug_log.h"
#include "shared/status_reporter.h"

namespace plain_text_perf_utils {

PlainTextStreamPerf::PlainTextStreamPerf()
    : stream_ms(0), chardata_ms(0), shape_ms(0), measure_ms(0),
      line_break_ms(0), pre_line_break_ms(0), chardata_calls(0),
      shape_calls(0), measure_calls(0), line_break_calls(0),
      pre_line_break_calls(0), shaped_glyphs(0), input_bytes(0),
      pages_generated(0), inline_images(0), page_overflows(0) {}

PlainTextPerfBaseline::PlainTextPerfBaseline()
    : chardata_ms(0), chardata_calls(0), inline_images(0), page_overflows(0),
      layout() {}

void CapturePlainTextPerfBaseline(const parsedata_t &parsedata,
                                  PlainTextPerfBaseline *out) {
  if (!out)
    return;
  out->chardata_ms = parsedata.perf_chardata_ms;
  out->chardata_calls = parsedata.perf_chardata_calls;
  out->inline_images = parsedata.perf_inline_images;
  out->page_overflows = parsedata.perf_page_overflows;
  out->layout = text_layout_utils::GetPerfStats();
}

void FillPlainTextStreamPerf(const parsedata_t &parsedata,
                             const PlainTextPerfBaseline &baseline,
                             u64 stream_ms, u32 input_bytes,
                             u16 pages_generated, PlainTextStreamPerf *out) {
  if (!out)
    return;
  const text_layout_utils::PerfStats layout_after =
      text_layout_utils::GetPerfStats();
  out->stream_ms = stream_ms;
  out->chardata_ms = parsedata.perf_chardata_ms - baseline.chardata_ms;
  out->shape_ms = layout_after.shape_ms - baseline.layout.shape_ms;
  out->measure_ms = layout_after.measure_ms - baseline.layout.measure_ms;
  out->line_break_ms =
      layout_after.line_break_ms - baseline.layout.line_break_ms;
  out->pre_line_break_ms =
      layout_after.pre_line_break_ms - baseline.layout.pre_line_break_ms;
  out->chardata_calls =
      parsedata.perf_chardata_calls - baseline.chardata_calls;
  out->shape_calls = layout_after.shape_calls - baseline.layout.shape_calls;
  out->measure_calls =
      layout_after.measure_calls - baseline.layout.measure_calls;
  out->line_break_calls =
      layout_after.line_break_calls - baseline.layout.line_break_calls;
  out->pre_line_break_calls = layout_after.pre_line_break_calls -
                              baseline.layout.pre_line_break_calls;
  out->shaped_glyphs =
      layout_after.shaped_glyphs - baseline.layout.shaped_glyphs;
  out->input_bytes = input_bytes;
  out->pages_generated = pages_generated;
  out->inline_images = parsedata.perf_inline_images - baseline.inline_images;
  out->page_overflows =
      parsedata.perf_page_overflows - baseline.page_overflows;
}

void LogPlainTextStreamPerf(IStatusReporter *reporter, const char *label,
                            const PlainTextStreamPerf &perf, bool completed) {
  if (!reporter || !label)
    return;
  DBG_LOGF(reporter,
           "%s: stream=%llums chardata=%llums/%u shape=%llums/%u "
           "break=%llums/%u pre=%llums/%u measure=%llums/%u "
           "bytes=%u glyphs=%u pages=%u inline_images=%u overflow_pages=%u "
           "done=%d",
           label, (unsigned long long)perf.stream_ms,
           (unsigned long long)perf.chardata_ms,
           (unsigned)perf.chardata_calls, (unsigned long long)perf.shape_ms,
           (unsigned)perf.shape_calls,
           (unsigned long long)perf.line_break_ms,
           (unsigned)perf.line_break_calls,
           (unsigned long long)perf.pre_line_break_ms,
           (unsigned)perf.pre_line_break_calls,
           (unsigned long long)perf.measure_ms,
           (unsigned)perf.measure_calls, (unsigned)perf.input_bytes,
           (unsigned)perf.shaped_glyphs, (unsigned)perf.pages_generated,
           (unsigned)perf.inline_images, (unsigned)perf.page_overflows,
           completed ? 1 : 0);
}

} // namespace plain_text_perf_utils


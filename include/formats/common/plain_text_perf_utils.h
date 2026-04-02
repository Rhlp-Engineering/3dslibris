/*
    3dslibris - plain_text_perf_utils.h

    Shared plain-text parse performance counters used by book_io paths.
*/

#pragma once

#include <3ds.h>

#include "parse.h"
#include "shared/text_layout_utils.h"

class IStatusReporter;

namespace plain_text_perf_utils {

struct PlainTextStreamPerf {
  u64 stream_ms;
  u64 chardata_ms;
  u64 shape_ms;
  u64 measure_ms;
  u64 line_break_ms;
  u64 pre_line_break_ms;
  u32 chardata_calls;
  u32 shape_calls;
  u32 measure_calls;
  u32 line_break_calls;
  u32 pre_line_break_calls;
  u32 shaped_glyphs;
  u32 input_bytes;
  u16 pages_generated;
  u32 inline_images;
  u32 page_overflows;

  PlainTextStreamPerf();
};

struct PlainTextPerfBaseline {
  u64 chardata_ms;
  u32 chardata_calls;
  u32 inline_images;
  u32 page_overflows;
  text_layout_utils::PerfStats layout;

  PlainTextPerfBaseline();
};

void CapturePlainTextPerfBaseline(const parsedata_t &parsedata,
                                  PlainTextPerfBaseline *out);
void FillPlainTextStreamPerf(const parsedata_t &parsedata,
                             const PlainTextPerfBaseline &baseline,
                             u64 stream_ms, u32 input_bytes,
                             u16 pages_generated, PlainTextStreamPerf *out);
void LogPlainTextStreamPerf(IStatusReporter *reporter, const char *label,
                            const PlainTextStreamPerf &perf, bool completed);

} // namespace plain_text_perf_utils

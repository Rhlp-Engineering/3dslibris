#include <cstddef>
#include <cstdint>
#include <cstdlib>

#include "mupdf/fitz/bidi.h"
#include "mupdf/fitz/context.h"

static bool IsRTLCodepoint(uint32_t cp) {
  if (cp >= 0x0590 && cp <= 0x05FF) return true;
  if (cp >= 0x0600 && cp <= 0x06FF) return true;
  if (cp >= 0x08A0 && cp <= 0x08FF) return true;
  if (cp >= 0xFB50 && cp <= 0xFDFF) return true;
  if (cp >= 0xFE70 && cp <= 0xFEFF) return true;
  return false;
}

extern "C" {

fz_context *fz_new_context(fz_alloc_context *, fz_alloc_context *,
                           size_t) {
  static int dummy;
  return (fz_context *)&dummy;
}

void fz_register_document_handlers(fz_context *) {}

void fz_bidi_fragment_text(fz_context *, const uint32_t *text, size_t textlen,
                           fz_bidi_direction *baseDir,
                           fz_bidi_fragment_fn *callback, void *arg,
                           int) {
  if (!text || textlen == 0 || !callback)
    return;

  size_t run_start = 0;
  bool run_is_rtl = IsRTLCodepoint(text[0]);

  for (size_t i = 1; i <= textlen; i++) {
    bool cur_rtl = (i < textlen) ? IsRTLCodepoint(text[i]) : !run_is_rtl;
    if (cur_rtl != run_is_rtl || i == textlen) {
      callback(text + run_start, i - run_start,
               run_is_rtl ? 1 : 0, 0, arg);
      run_start = i;
      run_is_rtl = cur_rtl;
    }
  }

  if (baseDir)
    *baseDir = IsRTLCodepoint(text[0]) ? FZ_BIDI_RTL : FZ_BIDI_LTR;
}

}

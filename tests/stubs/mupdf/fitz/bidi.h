#ifndef FITZ_BIDI_H
#define FITZ_BIDI_H

#include "mupdf/fitz/system.h"
#include "mupdf/fitz/context.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  FZ_BIDI_LTR = 0,
  FZ_BIDI_RTL = 1,
  FZ_BIDI_NEUTRAL = 2,
  FZ_BIDI_UNSET = 3
} fz_bidi_direction;

typedef enum {
  FZ_BIDI_CLASSIFY_WHITE_SPACE = 1,
  FZ_BIDI_REPLACE_TAB = 2
} fz_bidi_flags;

typedef void(fz_bidi_fragment_fn)(const uint32_t *fragment,
                                  size_t fragmentLen, int bidiLevel,
                                  int script, void *arg);

void fz_bidi_fragment_text(fz_context *ctx, const uint32_t *text,
                           size_t textlen, fz_bidi_direction *baseDir,
                           fz_bidi_fragment_fn *callback, void *arg,
                           int flags);

#ifdef __cplusplus
}
#endif

#endif

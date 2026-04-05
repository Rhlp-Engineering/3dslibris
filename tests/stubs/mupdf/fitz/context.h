#ifndef MUPDF_FITZ_CONTEXT_H
#define MUPDF_FITZ_CONTEXT_H

#include "mupdf/fitz/system.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct fz_alloc_context fz_alloc_context;
typedef struct fz_context fz_context;

fz_context *fz_new_context(fz_alloc_context *, fz_alloc_context *, size_t);
void fz_register_document_handlers(fz_context *);

#ifdef __cplusplus
}
#endif

#endif

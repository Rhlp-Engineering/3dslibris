#pragma once

#include "shared/app_flow_utils.h"

#include <cstddef>

namespace mupdf_render_policy_utils {

static const int kOld3dsPdfPreviewMaxXObjects = 128;
static const size_t kOld3dsPdfPreviewMaxContentBytes = 192u * 1024u;

inline bool ShouldSkipOld3dsPdfPreview(
    bool is_new_3ds, app_flow_utils::MuPdfDocumentKind document_kind,
    int xobject_count, size_t content_bytes) {
  if (is_new_3ds || document_kind != app_flow_utils::MuPdfDocumentKind::Pdf)
    return false;
  return xobject_count > kOld3dsPdfPreviewMaxXObjects ||
         content_bytes > kOld3dsPdfPreviewMaxContentBytes;
}

} // namespace mupdf_render_policy_utils

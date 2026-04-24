#include "formats/mupdf/mupdf_render_policy_utils.h"

#include <cstdio>
#include <cstdlib>
#include <string>

namespace {

[[noreturn]] void Fail(const std::string &message) {
  fprintf(stderr, "%s\n", message.c_str());
  std::exit(1);
}

void ExpectTrue(const char *label, bool value) {
  if (!value)
    Fail(std::string(label) + ": expected true");
}

void ExpectFalse(const char *label, bool value) {
  if (value)
    Fail(std::string(label) + ": expected false");
}

void TestOld3dsSkipsComplexPdfPreview() {
  using app_flow_utils::MuPdfDocumentKind;
  ExpectTrue("old3ds skips many xobjects",
             mupdf_render_policy_utils::ShouldSkipOld3dsPdfPreview(
                 false, MuPdfDocumentKind::Pdf,
                 mupdf_render_policy_utils::kOld3dsPdfPreviewMaxXObjects + 1,
                 1024));
  ExpectTrue("old3ds skips large content stream",
             mupdf_render_policy_utils::ShouldSkipOld3dsPdfPreview(
                 false, MuPdfDocumentKind::Pdf, 1,
                 mupdf_render_policy_utils::kOld3dsPdfPreviewMaxContentBytes +
                     1));
}

void TestPolicyKeepsNormalOrNonPdfPreview() {
  using app_flow_utils::MuPdfDocumentKind;
  ExpectFalse("old3ds keeps simple pdf",
              mupdf_render_policy_utils::ShouldSkipOld3dsPdfPreview(
                  false, MuPdfDocumentKind::Pdf, 4, 32u * 1024u));
  ExpectFalse("new3ds keeps complex pdf",
              mupdf_render_policy_utils::ShouldSkipOld3dsPdfPreview(
                  true, MuPdfDocumentKind::Pdf, 1000, 1024u * 1024u));
  ExpectFalse("old3ds keeps non-pdf",
              mupdf_render_policy_utils::ShouldSkipOld3dsPdfPreview(
                  false, MuPdfDocumentKind::Xps, 1000, 1024u * 1024u));
}

} // namespace

int main() {
  TestOld3dsSkipsComplexPdfPreview();
  TestPolicyKeepsNormalOrNonPdfPreview();
  return 0;
}

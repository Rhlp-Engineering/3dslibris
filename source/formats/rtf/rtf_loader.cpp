#include "formats/rtf/rtf_loader.h"
#include "formats/common/text_helpers.h"
#include "formats/common/file_read_utils.h"

namespace rtf_loader {

bool ReadAndDecode(const char* path, std::string* out_utf8) {
  std::string raw;
  if (!file_read_utils::ReadPathToStringLimited(path, &raw, kRtfMaxBytes)) {
    return false;
  }

  if (raw.empty()) {
    return false;
  }

  *out_utf8 = DecodeRtfToUtf8(raw);
  NormalizeNewlines(out_utf8);

  if (!LooksLikeValidUtf8Bytes(*out_utf8)) {
    *out_utf8 = NormalizeTextUtf8(*out_utf8);
  }

  return true;
}

} // namespace rtf_loader

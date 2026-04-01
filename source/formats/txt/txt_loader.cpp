#include "formats/txt/txt_loader.h"
#include "formats/common/text_helpers.h"
#include "formats/common/file_read_utils.h"

namespace txt_loader {

bool ReadAndNormalize(const char* path, std::string* out_utf8) {
    std::string raw;
    if (!file_read_utils::ReadPathToStringLimited(path, &raw, kTxtMaxBytes)) {
        return false;
    }

    if (raw.empty()) {
        return false;
    }

    NormalizeNewlines(&raw);
    *out_utf8 = NormalizeTextUtf8(std::move(raw));

    return true;
}

} // namespace txt_loader

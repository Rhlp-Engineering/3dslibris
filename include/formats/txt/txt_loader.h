#pragma once
#include <cstdint>
#include <string>

namespace txt_loader {

constexpr size_t kTxtMaxBytes = 12 * 1024 * 1024;

bool ReadAndNormalize(const char* path, std::string* out_utf8);

} // namespace txt_loader

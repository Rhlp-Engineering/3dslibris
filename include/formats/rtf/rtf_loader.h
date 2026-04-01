#pragma once
#include <cstdint>
#include <string>

namespace rtf_loader {

constexpr size_t kRtfMaxBytes = 12 * 1024 * 1024;

bool ReadAndDecode(const char* path, std::string* out_utf8);

} // namespace rtf_loader

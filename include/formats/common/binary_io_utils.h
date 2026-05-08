#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string>

namespace binary_io_utils {

bool WriteRaw(FILE *fp, const void *data, size_t bytes);
bool ReadRaw(FILE *fp, void *data, size_t bytes);

bool WriteString16(FILE *fp, const std::string &value,
                   uint16_t max_bytes);
bool ReadString16(FILE *fp, uint16_t max_bytes, std::string *out);

bool WriteString32(FILE *fp, const std::string &value,
                   uint32_t max_bytes);
bool ReadString32(FILE *fp, uint32_t max_bytes, std::string *out);

} // namespace binary_io_utils

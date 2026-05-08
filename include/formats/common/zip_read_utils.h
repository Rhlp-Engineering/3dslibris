#pragma once

#include "minizip/unzip.h"

#include <stddef.h>
#include <string>
#include <vector>

namespace zip_read_utils {

bool ReadCurrentEntryBinary(unzFile uf, unsigned long uncompressed_size,
                            std::vector<unsigned char> *out,
                            size_t max_bytes,
                            const char **error_message);

bool ReadEntryBinary(unzFile uf, const std::string &path,
                     std::vector<unsigned char> *out,
                     size_t max_bytes,
                     const char **error_message);

} // namespace zip_read_utils

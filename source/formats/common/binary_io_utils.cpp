#include "formats/common/binary_io_utils.h"

namespace binary_io_utils {

bool WriteRaw(FILE *fp, const void *data, size_t bytes) {
  if (!fp)
    return false;
  if (bytes == 0)
    return true;
  if (!data)
    return false;
  return fwrite(data, 1, bytes, fp) == bytes;
}

bool ReadRaw(FILE *fp, void *data, size_t bytes) {
  if (!fp)
    return false;
  if (bytes == 0)
    return true;
  if (!data)
    return false;
  return fread(data, 1, bytes, fp) == bytes;
}

bool WriteString16(FILE *fp, const std::string &value,
                   uint16_t max_bytes) {
  if (value.size() > max_bytes)
    return false;
  const uint16_t length = (uint16_t)value.size();
  return WriteRaw(fp, &length, sizeof(length)) &&
         WriteRaw(fp, value.data(), value.size());
}

bool ReadString16(FILE *fp, uint16_t max_bytes, std::string *out) {
  if (!out)
    return false;
  out->clear();
  uint16_t length = 0;
  if (!ReadRaw(fp, &length, sizeof(length)) || length > max_bytes)
    return false;
  if (length == 0)
    return true;
  out->resize(length);
  return ReadRaw(fp, &(*out)[0], length);
}

bool WriteString32(FILE *fp, const std::string &value,
                   uint32_t max_bytes) {
  if (value.size() > max_bytes)
    return false;
  const uint32_t length = (uint32_t)value.size();
  return WriteRaw(fp, &length, sizeof(length)) &&
         WriteRaw(fp, value.data(), value.size());
}

bool ReadString32(FILE *fp, uint32_t max_bytes, std::string *out) {
  if (!out)
    return false;
  out->clear();
  uint32_t length = 0;
  if (!ReadRaw(fp, &length, sizeof(length)) || length > max_bytes)
    return false;
  if (length == 0)
    return true;
  out->resize(length);
  return ReadRaw(fp, &(*out)[0], length);
}

} // namespace binary_io_utils

#include "formats/mobi/mobi_record_scan.h"

#include <algorithm>

namespace mobi_record_scan {

std::uint16_t ReadBE16(const unsigned char *p) {
  return (std::uint16_t)((std::uint16_t)p[0] << 8 | (std::uint16_t)p[1]);
}

std::uint32_t ReadBE32(const unsigned char *p) {
  return (std::uint32_t)((std::uint32_t)p[0] << 24 | (std::uint32_t)p[1] << 16 |
                         (std::uint32_t)p[2] << 8 | (std::uint32_t)p[3]);
}

bool ParseRecordOffsets(const std::string &raw, std::vector<std::uint32_t> *offsets) {
  if (!offsets || raw.size() < 78)
    return false;

  const unsigned char *buf = (const unsigned char *)raw.data();
  const std::uint16_t rec_count = ReadBE16(buf + 76);
  if (rec_count == 0 || raw.size() < 78 + (size_t)rec_count * 8)
    return false;

  offsets->assign((size_t)rec_count + 1, 0);
  for (std::uint16_t i = 0; i < rec_count; i++) {
    std::uint32_t off = ReadBE32(buf + 78 + (size_t)i * 8);
    if (off >= raw.size())
      return false;
    if (i > 0 && off < (*offsets)[(size_t)i - 1])
      return false;
    (*offsets)[i] = off;
  }
  (*offsets)[(size_t)rec_count] = (std::uint32_t)raw.size();
  return true;
}

unsigned FirstImageProbeLimit(unsigned remaining_records) {
  return std::min(remaining_records, 128u);
}

unsigned CoverLastResortProbeLimit(unsigned remaining_records) {
  return std::min(remaining_records, 192u);
}

} // namespace mobi_record_scan

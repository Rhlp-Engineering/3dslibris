#include "reader/fixed_layout_input_utils.h"

namespace reader_input_utils {

bool FixedLayoutSupportsShoulderPageTurn(format_t format) {
  return format == FORMAT_PDF || format == FORMAT_CBZ;
}

} // namespace reader_input_utils

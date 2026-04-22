#include "reader/reflow_open_gate_utils.h"

namespace reader {

bool ShouldUseAsyncReflowOpen(bool uses_text_layout_settings) {
  return uses_text_layout_settings;
}

} // namespace reader

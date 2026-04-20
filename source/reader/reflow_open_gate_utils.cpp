#include "reader/reflow_open_gate_utils.h"

namespace reader {

bool ShouldUseAsyncReflowOpen(bool uses_text_layout_settings,
                              bool is_mobi_file) {
  return uses_text_layout_settings && !is_mobi_file;
}

} // namespace reader

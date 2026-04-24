#include "shared/open_cancel_poll_utils.h"

namespace open_cancel_poll_utils {

bool ShouldCancel(bool reporter_abort_requested, bool book_abort_requested) {
  return reporter_abort_requested || book_abort_requested;
}

} // namespace open_cancel_poll_utils

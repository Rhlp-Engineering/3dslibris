#include "reader/suspend_policy_utils.h"

namespace reader_suspend_policy_utils {

bool ShouldKeepOpeningDuringSuspend(bool has_opening_book,
                                     bool async_reflow_open_pending) {
  // If async reflow open is pending, we should NOT keep opening during suspend.
  // Synchronous opens are also canceled. We never keep opening during suspend.
  return false;
}

} // namespace reader_suspend_policy_utils

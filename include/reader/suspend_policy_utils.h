#pragma once

namespace reader_suspend_policy_utils {

bool ShouldKeepOpeningDuringSuspend(bool has_opening_book,
                                    bool async_reflow_open_pending);

} // namespace reader_suspend_policy_utils

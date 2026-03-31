#pragma once

#include <cstdint>

namespace browser_warmup_utils {

static const uint64_t kBrowserWarmupIdleDelayMs = 250;

inline bool IsBrowserWarmupIdle(uint64_t now_ms, uint64_t last_interaction_ms,
                                bool wait_input_release) {
  if (wait_input_release)
    return false;
  if (now_ms < last_interaction_ms)
    return false;
  return (now_ms - last_interaction_ms) >= kBrowserWarmupIdleDelayMs;
}

} // namespace browser_warmup_utils

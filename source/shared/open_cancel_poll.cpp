#include "shared/open_cancel_poll.h"

#include <3ds.h>

#include "book/book.h"
#include "debug_log.h"
#include "shared/status_reporter.h"

namespace open_cancel_poll {
namespace {

static u64 g_last_poll_ms = 0;
static const u32 kOpenPollIntervalMs = 48;

} // namespace

void Reset() { g_last_poll_ms = 0; }

bool Poll(Book *book, IStatusReporter *reporter, const char *stage) {
  (void)stage;
  if (!book)
    return reporter && reporter->ShouldAbortWork();
  if ((reporter && reporter->ShouldAbortWork()) || book->IsOpenAbortRequested())
    return true;

  // This helper may run inside long-lived parser/layout code and, in newer
  // branches, on worker threads. Keep it side-effect free and rely on
  // ShouldAbortWork()/RequestAbortOpen() to propagate suspend or cancel.
  const u64 now = osGetTime();
  if (g_last_poll_ms != 0 && now >= g_last_poll_ms &&
      (now - g_last_poll_ms) < kOpenPollIntervalMs)
    return false;
  g_last_poll_ms = now;

  return (reporter && reporter->ShouldAbortWork()) ||
         book->IsOpenAbortRequested();
}

} // namespace open_cancel_poll

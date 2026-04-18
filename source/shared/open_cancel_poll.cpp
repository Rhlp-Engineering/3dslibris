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
  if (!book)
    return reporter && reporter->ShouldAbortWork();
  if ((reporter && reporter->ShouldAbortWork()) || book->IsOpenAbortRequested())
    return true;

  const u64 now = osGetTime();
  if (g_last_poll_ms != 0 && now >= g_last_poll_ms &&
      (now - g_last_poll_ms) < kOpenPollIntervalMs)
    return false;
  g_last_poll_ms = now;

  gspWaitForVBlank();
  if (!aptMainLoop()) {
    book->RequestAbortOpen();
#ifdef DSLIBRIS_DEBUG
    if (reporter)
      DBG_LOGF(reporter, "BOOK open poll: applet-exit stage=%s",
               stage ? stage : "");
#endif
    return true;
  }

  hidScanInput();
  const u32 keys = hidKeysDown();
  if (keys & (KEY_B | KEY_START | KEY_SELECT)) {
    book->RequestAbortOpen();
    if (reporter)
      reporter->PrintStatus("cancelling open ...");
#ifdef DSLIBRIS_DEBUG
    if (reporter) {
      DBG_LOGF(reporter, "BOOK open poll: cancel keys=0x%08lx stage=%s",
               (unsigned long)keys, stage ? stage : "");
    }
#endif
    return true;
  }

  return (reporter && reporter->ShouldAbortWork()) ||
         book->IsOpenAbortRequested();
}

} // namespace open_cancel_poll

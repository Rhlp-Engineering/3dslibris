#include "shared/open_cancel_poll.h"

#include "book/book.h"
#include "shared/open_cancel_poll_utils.h"
#include "shared/status_reporter.h"

namespace open_cancel_poll {

void Reset() {}

bool Poll(Book *book, IStatusReporter *reporter, const char *stage) {
  (void)stage;
  const bool reporter_abort_requested =
      reporter && reporter->ShouldAbortWork();
  if (!book)
    return open_cancel_poll_utils::ShouldCancel(reporter_abort_requested,
                                                false);
  const bool book_abort_requested = book->IsOpenAbortRequested();
  if (open_cancel_poll_utils::ShouldCancel(reporter_abort_requested,
                                           book_abort_requested))
    return true;

  // This helper may run inside worker threads. Keep it side-effect free and
  // stateless so callers can poll aggressively without touching libctru state
  // or sharing mutable globals across threads.

  return open_cancel_poll_utils::ShouldCancel(reporter_abort_requested,
                                              book_abort_requested);
}

} // namespace open_cancel_poll

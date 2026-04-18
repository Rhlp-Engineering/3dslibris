#pragma once

class Book;
class IStatusReporter;

namespace open_cancel_poll {

void Reset();
bool Poll(Book *book, IStatusReporter *reporter, const char *stage);

} // namespace open_cancel_poll

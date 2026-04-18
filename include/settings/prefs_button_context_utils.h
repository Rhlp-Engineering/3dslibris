#pragma once

#include "settings/prefs_button_ids.h"

namespace settings {

unsigned char VisiblePrefsButtonCount(bool from_book, bool include_book_option);
int PrefsButtonForVisibleSlot(bool from_book, bool include_book_option,
                              unsigned char slot);

} // namespace settings

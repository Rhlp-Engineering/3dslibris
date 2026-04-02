/*
    3dslibris - page_text_extract_utils.h

    Helpers to extract plain text lines from parsed pages.
*/

#pragma once

#include <string>
#include <vector>

class Page;

namespace page_text_extract_utils {

std::vector<std::string> ExtractTextLinesFromPage(Page *page);

} // namespace page_text_extract_utils


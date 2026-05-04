#include "book/book_xml_flow_layout.h"
#include <cassert>

static void test_null_safety() {
    book_xml_flow_layout::AdvanceParsedPageOnOverflow(nullptr, 16);
    book_xml_flow_layout::AdvanceParsedPageOnOverflowThunk(nullptr, 16, nullptr);
    book_xml_flow_layout::AdvanceParsedScreen(nullptr);
    book_xml_flow_layout::ForcePageBreak(nullptr);
}

static void test_thunk_ctx_ignored() {
    int dummy = 42;
    book_xml_flow_layout::AdvanceParsedPageOnOverflowThunk(nullptr, 0, &dummy);
}

int main() {
    test_null_safety();
    test_thunk_ctx_ignored();
    return 0;
}

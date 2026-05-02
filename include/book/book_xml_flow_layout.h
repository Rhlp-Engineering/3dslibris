#pragma once

#include "parse.h"

namespace book_xml_flow_layout {

void AdvanceParsedPageOnOverflow(parsedata_t *p, int lineheight);
void AdvanceParsedPageOnOverflowThunk(parsedata_t *p, int lineheight, void *ctx);
void AdvanceParsedScreen(parsedata_t *p);
void ForcePageBreak(parsedata_t *p);

} // namespace book_xml_flow_layout

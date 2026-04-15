#pragma once

#include "book/book_xml_css_style_utils.h"

#include <map>
#include <string>

namespace epub_css_class_map {

using book_xml_css_style_utils::MarginTopResult;

struct CssClassMargins {
  MarginTopResult margin_top;
  MarginTopResult margin_bottom;
  bool hide_list_markers;

  CssClassMargins() : hide_list_markers(false) {}
};

// Map: bare class name (no '.') → extracted margins.
using CssClassMap = std::map<std::string, CssClassMargins>;

// Parses raw CSS text; populates *out with class selectors that have
// margin-top/bottom rules. Other selectors and properties are skipped.
// Safe with css_text==nullptr or len==0.
void ParseCssIntoClassMap(const char *css_text, size_t len, CssClassMap *out);

// Resolves a raw HTML class="" attribute against the parsed CSS class map.
// Multiple whitespace-separated classes are supported. If several known
// classes match, their top/bottom margins are merged into *out.
bool LookupMarginsForClassAttr(const std::string &class_attr,
                               const CssClassMap &class_map,
                               CssClassMargins *out);

bool LookupHideListMarkersForClassAttr(const std::string &class_attr,
                                       const CssClassMap &class_map);

} // namespace epub_css_class_map

/*
    3dslibris - string_utils.cpp
    Shared string utilities extracted from duplicated static functions.
    Created by Rigle to reduce code duplication.
*/

#include "shared/string_utils.h"
#include <string.h>
#include <string>

const char *BrowserDisplayNameSource(const char *title, const char *filename) {
  if (!title || title[0] == '\0')
    return filename ? filename : "";
  std::string stem = filename ? filename : "";
  if (stem.empty())
    return title;
  size_t dot = stem.find_last_of('.');
  if (dot != std::string::npos)
    stem = stem.substr(0, dot);
  std::string t(title);
  std::string s(stem);
  for (size_t i = 0; i < t.size(); i++)
    if (t[i] >= 'A' && t[i] <= 'Z')
      t[i] = (char)(t[i] - 'A' + 'a');
  for (size_t i = 0; i < s.size(); i++)
    if (s[i] >= 'A' && s[i] <= 'Z')
      s[i] = (char)(s[i] - 'A' + 'a');
  if (t == s)
    return filename ? filename : "";
  return title;
}

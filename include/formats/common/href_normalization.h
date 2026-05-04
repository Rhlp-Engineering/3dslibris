#ifndef FORMATS_COMMON_HREF_NORMALIZATION_H
#define FORMATS_COMMON_HREF_NORMALIZATION_H

#include <string>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <ctype.h>

namespace href_normalization {

std::string BasenamePathLocal(const std::string &path);
std::string AnchorTokenKey(const std::string &s);
std::string AnchorDigits(const std::string &s);
std::string NormalizePathForAnchor(const std::string &path);
std::string UrlDecodeComponent(const std::string &input);
std::string ToLowerAsciiLocal(const std::string &s);
std::string BuildAnchorKey(const std::string &docpath, const std::string &anchor_raw);
std::string NormalizeAnchorHrefKey(const std::string &href);
std::string NormalizeDocStartPathKey(const std::string &raw_path, bool strip_fragment_and_query);

} // namespace href_normalization

#endif // FORMATS_COMMON_HREF_NORMALIZATION_H
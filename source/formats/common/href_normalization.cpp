/*
    3dslibris - href_normalization.cpp

    Pure functions for normalizing EPUB/MOBI hrefs, anchors, fragments,
    and internal document paths. Extracted from book.cpp.
*/

#include "formats/common/href_normalization.h"

#include "shared/string_utils.h"

namespace href_normalization {
namespace {

int HexDigit(char c) {
  if (c >= '0' && c <= '9')
    return c - '0';
  if (c >= 'a' && c <= 'f')
    return 10 + c - 'a';
  if (c >= 'A' && c <= 'F')
    return 10 + c - 'A';
  return -1;
}

} // namespace

std::string BasenamePathLocal(const std::string &path) {
  size_t slash = path.find_last_of('/');
  if (slash == std::string::npos)
    return path;
  if (slash + 1 >= path.size())
    return "";
  return path.substr(slash + 1);
}

std::string AnchorTokenKey(const std::string &s) {
  std::string out;
  out.reserve(s.size());
  for (size_t i = 0; i < s.size(); i++) {
    unsigned char c = (unsigned char)s[i];
    if ((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9'))
      out.push_back((char)c);
  }
  return out;
}

std::string AnchorDigits(const std::string &s) {
  std::string out;
  out.reserve(s.size());
  for (size_t i = 0; i < s.size(); i++) {
    unsigned char c = (unsigned char)s[i];
    if (c >= '0' && c <= '9')
      out.push_back((char)c);
  }
  return out;
}

std::string NormalizePathForAnchor(const std::string &path) {
  std::string in = path;
  std::replace(in.begin(), in.end(), '\\', '/');
  while (!in.empty() && in[0] == '/')
    in.erase(in.begin());

  std::vector<std::string> parts;
  std::string cur;
  for (size_t i = 0; i <= in.size(); i++) {
    if (i == in.size() || in[i] == '/') {
      if (cur == "..") {
        if (!parts.empty())
          parts.pop_back();
      } else if (!cur.empty() && cur != ".") {
        parts.push_back(cur);
      }
      cur.clear();
    } else {
      cur.push_back(in[i]);
    }
  }

  std::string out;
  for (size_t i = 0; i < parts.size(); i++) {
    if (i)
      out.push_back('/');
    out += parts[i];
  }
  return out;
}

std::string UrlDecodeComponent(const std::string &input) {
  std::string out;
  out.reserve(input.size());
  for (size_t i = 0; i < input.size(); i++) {
    if (input[i] == '%' && i + 2 < input.size()) {
      const int hi = HexDigit(input[i + 1]);
      const int lo = HexDigit(input[i + 2]);
      if (hi >= 0 && lo >= 0) {
        out.push_back((char)((hi << 4) | lo));
        i += 2;
        continue;
      }
    }
    out.push_back(input[i]);
  }
  return out;
}

std::string ToLowerAsciiLocal(const std::string &s) {
  return ToLowerAscii(s);
}

std::string BuildAnchorKey(const std::string &docpath,
                           const std::string &anchor_raw) {
  if (docpath.empty() || anchor_raw.empty())
    return "";

  std::string doc = UrlDecodeComponent(docpath);
  std::string anchor = UrlDecodeComponent(anchor_raw);
  while (!anchor.empty() && anchor[0] == '#')
    anchor.erase(anchor.begin());
  if (anchor.empty())
    return "";
  if (anchor.size() > 512)
    anchor.resize(512);

  std::string key = NormalizePathForAnchor(doc);
  if (key.empty())
    return "";
  key.push_back('#');
  key += anchor;
  return key;
}

std::string NormalizeAnchorHrefKey(const std::string &href) {
  if (href.empty())
    return "";
  std::string decoded = UrlDecodeComponent(href);
  size_t hash = decoded.find('#');
  if (hash == std::string::npos || hash + 1 >= decoded.size())
    return "";
  std::string path = decoded.substr(0, hash);
  std::string anchor = decoded.substr(hash + 1);
  size_t q = anchor.find('?');
  if (q != std::string::npos)
    anchor = anchor.substr(0, q);
  return BuildAnchorKey(path, anchor);
}

std::string NormalizeDocStartPathKey(const std::string &raw_path,
                                     bool strip_fragment_and_query) {
  if (raw_path.empty())
    return "";

  std::string decoded = UrlDecodeComponent(raw_path);
  if (strip_fragment_and_query) {
    size_t hash = decoded.find('#');
    if (hash != std::string::npos)
      decoded = decoded.substr(0, hash);
    size_t q = decoded.find('?');
    if (q != std::string::npos)
      decoded = decoded.substr(0, q);
  }

  return NormalizePathForAnchor(decoded);
}

} // namespace href_normalization

#ifndef _MOBI_CACHE_UTILS_H_
#define _MOBI_CACHE_UTILS_H_

#include <vector>

namespace mobi_cache_utils {

template <typename T>
bool CopyCachedVectorIfReady(const std::vector<T> &cached, bool have_cached,
                             std::vector<T> *out) {
  if (!out)
    return false;
  if (!have_cached || cached.empty())
    return false;
  if (out != &cached)
    *out = cached;
  return true;
}

} // namespace mobi_cache_utils

#endif

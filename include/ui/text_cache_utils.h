#pragma once

namespace text_cache_utils {

template <typename Face, typename CacheMap>
typename CacheMap::mapped_type FindFaceCache(const CacheMap &caches, Face face) {
  if (!face)
    return nullptr;

  typename CacheMap::const_iterator it = caches.find(face);
  if (it == caches.end())
    return nullptr;

  return it->second;
}

} // namespace text_cache_utils

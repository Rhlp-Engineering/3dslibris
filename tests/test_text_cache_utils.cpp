#include "ui/text_cache_utils.h"

#include <cstdio>
#include <cstdlib>
#include <map>
#include <string>

namespace {

struct DummyCache {
  int value;
};

[[noreturn]] void Fail(const std::string &message) {
  fprintf(stderr, "%s\n", message.c_str());
  std::exit(1);
}

void ExpectTrue(const char *label, bool value) {
  if (!value)
    Fail(std::string(label) + ": expected true");
}

void ExpectFalse(const char *label, bool value) {
  if (value)
    Fail(std::string(label) + ": expected false");
}

void ExpectEq(const char *label, size_t actual, size_t expected) {
  if (actual != expected) {
    Fail(std::string(label) + ": expected " + std::to_string(expected) +
         ", got " + std::to_string(actual));
  }
}

void ExpectPtrEq(const char *label, const void *actual, const void *expected) {
  if (actual != expected)
    Fail(std::string(label) + ": unexpected pointer result");
}

void TestMissingLookupDoesNotInsert() {
  std::map<void *, DummyCache *> caches;
  void *face = reinterpret_cast<void *>(0x1234);
  ExpectPtrEq("missing cache lookup", text_cache_utils::FindFaceCache(caches, face),
              nullptr);
  ExpectEq("missing lookup should not grow map", caches.size(), 0);
}

void TestExistingLookupReturnsCache() {
  std::map<void *, DummyCache *> caches;
  DummyCache cache = {42};
  void *face = reinterpret_cast<void *>(0x4321);
  caches[face] = &cache;

  ExpectPtrEq("existing cache lookup", text_cache_utils::FindFaceCache(caches, face),
              &cache);
  ExpectEq("existing lookup keeps map size", caches.size(), 1);
}

void TestNullFaceAlwaysReturnsNull() {
  std::map<void *, DummyCache *> caches;
  DummyCache cache = {7};
  caches[reinterpret_cast<void *>(0x9999)] = &cache;
  ExpectPtrEq("null face lookup",
              text_cache_utils::FindFaceCache(caches,
                                              static_cast<void *>(nullptr)),
              nullptr);
  ExpectEq("null face leaves map untouched", caches.size(), 1);
}

} // namespace

int main() {
  TestMissingLookupDoesNotInsert();
  TestExistingLookupReturnsCache();
  TestNullFaceAlwaysReturnsNull();
  return 0;
}

#include "formats/mobi/mobi_cache_utils.h"

#include <cstdlib>
#include <string>
#include <vector>

namespace {

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

void ExpectEqSize(const char *label, size_t actual, size_t expected) {
  if (actual != expected)
    Fail(std::string(label) + ": unexpected size");
}

void TestCopyCachedVector() {
  std::vector<int> cached;
  cached.push_back(7);
  cached.push_back(9);
  std::vector<int> out;
  out.push_back(1);

  ExpectTrue("copy cached vector",
             mobi_cache_utils::CopyCachedVectorIfReady(cached, true, &out));
  ExpectEqSize("copied size", out.size(), 2u);
}

void TestAliasKeepsCachedEntries() {
  std::vector<int> cached;
  cached.push_back(3);
  cached.push_back(5);

  ExpectTrue("alias copy cached",
             mobi_cache_utils::CopyCachedVectorIfReady(cached, true, &cached));
  ExpectEqSize("alias size preserved", cached.size(), 2u);
}

void TestMissingCacheDoesNothing() {
  std::vector<int> out;
  out.push_back(4);
  ExpectFalse("missing cache",
              mobi_cache_utils::CopyCachedVectorIfReady(out, false, &out));
  ExpectEqSize("missing cache preserved", out.size(), 1u);
}

} // namespace

int main() {
  TestCopyCachedVector();
  TestAliasKeepsCachedEntries();
  TestMissingCacheDoesNothing();
  return 0;
}

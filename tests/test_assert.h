#pragma once

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <string>

namespace test {

[[noreturn]] inline void Fail(const std::string &message) {
  fprintf(stderr, "FAIL: %s\n", message.c_str());
  std::exit(1);
}

inline void ExpectEq(const char *label, int actual, int expected) {
  if (actual != expected) {
    Fail(std::string(label) + ": expected " + std::to_string(expected) +
         ", got " + std::to_string(actual));
  }
}

inline void ExpectEqU(const char *label, unsigned int actual, unsigned int expected) {
  if (actual != expected) {
    Fail(std::string(label) + ": expected " + std::to_string(expected) +
         ", got " + std::to_string(actual));
  }
}

inline void ExpectEqU64(const char *label, unsigned long long actual, unsigned long long expected) {
  if (actual != expected) {
    Fail(std::string(label) + ": expected " + std::to_string(expected) +
         ", got " + std::to_string(actual));
  }
}

inline void ExpectTrue(const char *label, bool value) {
  if (!value)
    Fail(std::string(label) + ": expected true");
}

inline void ExpectFalse(const char *label, bool value) {
  if (value)
    Fail(std::string(label) + ": expected false");
}

inline void ExpectStrEq(const char *label, const char *actual, const char *expected) {
  if (std::strcmp(actual, expected) != 0) {
    Fail(std::string(label) + ": expected \"" + std::string(expected) +
         "\", got \"" + std::string(actual) + "\"");
  }
}

inline void ExpectStrContains(const char *label, const char *haystack, const char *needle) {
  if (std::strstr(haystack, needle) == nullptr) {
    Fail(std::string(label) + ": expected to contain \"" + std::string(needle) +
         "\" in \"" + std::string(haystack) + "\"");
  }
}

inline void ExpectNe(const char *label, int actual, int expected) {
  if (actual == expected) {
    Fail(std::string(label) + ": expected != " + std::to_string(expected));
  }
}

inline void ExpectLt(const char *label, int actual, int expected) {
  if (!(actual < expected)) {
    Fail(std::string(label) + ": expected " + std::to_string(actual) +
         " < " + std::to_string(expected));
  }
}

inline void ExpectGt(const char *label, int actual, int expected) {
  if (!(actual > expected)) {
    Fail(std::string(label) + ": expected " + std::to_string(actual) +
         " > " + std::to_string(expected));
  }
}

inline void ExpectNotNull(const char *label, const void *ptr) {
  if (!ptr)
    Fail(std::string(label) + ": expected non-null pointer");
}

} // namespace test

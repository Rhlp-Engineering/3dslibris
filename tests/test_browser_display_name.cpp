#include "../include/shared/string_utils.h"
#include "test_assert.h"

static void TestEmptyTitle() {
  const char *r = BrowserDisplayNameSource("", "foo.epub");
  test::ExpectStrEq("empty title", r, "foo.epub");
}

static void TestNullTitle() {
  const char *r = BrowserDisplayNameSource(nullptr, "foo.epub");
  test::ExpectNotNull("null title result", r);
  test::ExpectStrEq("null title", r, "foo.epub");
}

static void TestTitleEqualsStem() {
  const char *r = BrowserDisplayNameSource("foo", "foo.epub");
  test::ExpectStrEq("title equals stem", r, "foo.epub");
}

static void TestTitleEqualsStemCaseInsensitive() {
  const char *r = BrowserDisplayNameSource("Foo", "foo.epub");
  test::ExpectStrEq("title equals stem case-insensitive", r, "foo.epub");
}

static void TestTitleDifferentFromStem() {
  const char *r = BrowserDisplayNameSource("My Book", "foo.epub");
  test::ExpectStrEq("title differs from stem", r, "My Book");
}

static void TestTitleWithSpecialChars() {
  const char *r = BrowserDisplayNameSource("Cien a\xc3\xb1os de soledad", "cien_anios.epub");
  test::ExpectStrEq("title with special chars", r, "Cien a\xc3\xb1os de soledad");
}

static void TestNullFilename() {
  const char *r = BrowserDisplayNameSource("My Book", nullptr);
  test::ExpectNotNull("null filename result", r);
  test::ExpectStrEq("null filename", r, "My Book");
}

static void TestEmptyFilename() {
  const char *r = BrowserDisplayNameSource("My Book", "");
  test::ExpectStrEq("empty filename", r, "My Book");
}

int main() {
  TestEmptyTitle();
  TestNullTitle();
  TestTitleEqualsStem();
  TestTitleEqualsStemCaseInsensitive();
  TestTitleDifferentFromStem();
  TestTitleWithSpecialChars();
  TestNullFilename();
  TestEmptyFilename();
  printf("All BrowserDisplayNameSource tests passed.\n");
  return 0;
}

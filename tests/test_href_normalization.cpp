#include <stdio.h>
#include <string.h>
#include "formats/common/href_normalization.h"

static int pass_count = 0;
static int fail_count = 0;

#define TEST(name) static bool name()
#define RUN_TEST(name) do { \
  if (name()) { pass_count++; printf("  PASS: %s\n", #name); } \
  else { fail_count++; printf("  FAIL: %s\n", #name); } \
} while(0)

#define ASSERT_EQ(a, b) do { \
  if ((a) != (b)) { printf("    %s:%d: %s != %s\n", __FILE__, __LINE__, #a, #b); return false; } \
} while(0)

#define ASSERT_STR_EQ(a, b) do { \
  std::string _a = (a); std::string _b = (b); \
  if (_a != _b) { printf("    %s:%d: \"%s\" != \"%s\"\n", __FILE__, __LINE__, _a.c_str(), _b.c_str()); return false; } \
} while(0)

TEST(test_basename_root) {
  ASSERT_STR_EQ(href_normalization::BasenamePathLocal("file.xhtml"), "file.xhtml");
  ASSERT_STR_EQ(href_normalization::BasenamePathLocal(""), "");
  return true;
}

TEST(test_basename_with_path) {
  ASSERT_STR_EQ(href_normalization::BasenamePathLocal("OEBPS/text/chapter.xhtml"), "chapter.xhtml");
  ASSERT_STR_EQ(href_normalization::BasenamePathLocal("/OEBPS/text/chapter.xhtml"), "chapter.xhtml");
  return true;
}

TEST(test_basename_trailing_slash) {
  ASSERT_STR_EQ(href_normalization::BasenamePathLocal("OEBPS/text/"), "");
  return true;
}

TEST(test_anchor_token_key) {
  // Filters: keeps only lowercase ASCII + digits. Uppercase/symbols are DROPPED, not transformed.
  ASSERT_STR_EQ(href_normalization::AnchorTokenKey("section1"), "section1");
  ASSERT_STR_EQ(href_normalization::AnchorTokenKey("Section-1"), "ection1"); // 'S' dropped, not lowercased
  ASSERT_STR_EQ(href_normalization::AnchorTokenKey("a_b_c"), "abc");
  ASSERT_STR_EQ(href_normalization::AnchorTokenKey(""), "");
  return true;
}

TEST(test_anchor_digits) {
  ASSERT_STR_EQ(href_normalization::AnchorDigits("chapter123"), "123");
  ASSERT_STR_EQ(href_normalization::AnchorDigits("no_digits"), "");
  ASSERT_STR_EQ(href_normalization::AnchorDigits("a1b2c3"), "123");
  return true;
}

TEST(test_normalize_path_empty) {
  ASSERT_STR_EQ(href_normalization::NormalizePathForAnchor(""), "");
  return true;
}

TEST(test_normalize_path_leading_slashes) {
  ASSERT_STR_EQ(href_normalization::NormalizePathForAnchor("/OEBPS/text/ch.xhtml"), "OEBPS/text/ch.xhtml");
  ASSERT_STR_EQ(href_normalization::NormalizePathForAnchor("//OEBPS"), "OEBPS");
  return true;
}

TEST(test_normalize_path_dotdot) {
  ASSERT_STR_EQ(href_normalization::NormalizePathForAnchor("OEBPS/../text/ch.xhtml"), "text/ch.xhtml");
  ASSERT_STR_EQ(href_normalization::NormalizePathForAnchor("a/b/../../c"), "c");
  return true;
}

TEST(test_normalize_path_dots) {
  ASSERT_STR_EQ(href_normalization::NormalizePathForAnchor("./OEBPS/ch.xhtml"), "OEBPS/ch.xhtml");
  ASSERT_STR_EQ(href_normalization::NormalizePathForAnchor("a/./b"), "a/b");
  return true;
}

TEST(test_normalize_path_backslashes) {
  ASSERT_STR_EQ(href_normalization::NormalizePathForAnchor("OEBPS\\text\\ch.xhtml"), "OEBPS/text/ch.xhtml");
  return true;
}

TEST(test_normalize_path_already_normalized) {
  ASSERT_STR_EQ(href_normalization::NormalizePathForAnchor("OEBPS/text/ch.xhtml"), "OEBPS/text/ch.xhtml");
  return true;
}

TEST(test_url_decode_space) {
  ASSERT_STR_EQ(href_normalization::UrlDecodeComponent("hello%20world"), "hello world");
  return true;
}

TEST(test_url_decode_hash) {
  ASSERT_STR_EQ(href_normalization::UrlDecodeComponent("test%23anchor"), "test#anchor");
  return true;
}

TEST(test_url_decode_no_encoding) {
  ASSERT_STR_EQ(href_normalization::UrlDecodeComponent("plain"), "plain");
  return true;
}

TEST(test_url_decode_partial) {
  ASSERT_STR_EQ(href_normalization::UrlDecodeComponent("ab%"), "ab%");
  ASSERT_STR_EQ(href_normalization::UrlDecodeComponent("ab%2"), "ab%2");
  return true;
}

TEST(test_to_lower_ascii) {
  ASSERT_STR_EQ(href_normalization::ToLowerAsciiLocal("HELLO"), "hello");
  ASSERT_STR_EQ(href_normalization::ToLowerAsciiLocal("Mixed123"), "mixed123");
  ASSERT_STR_EQ(href_normalization::ToLowerAsciiLocal(""), "");
  return true;
}

TEST(test_build_anchor_key_normal) {
  std::string key = href_normalization::BuildAnchorKey("OEBPS/text/ch.xhtml", "section1");
  ASSERT_STR_EQ(key, "OEBPS/text/ch.xhtml#section1");
  return true;
}

TEST(test_build_anchor_key_empty_docpath) {
  ASSERT_STR_EQ(href_normalization::BuildAnchorKey("", "anchor"), "");
  return true;
}

TEST(test_build_anchor_key_empty_anchor) {
  ASSERT_STR_EQ(href_normalization::BuildAnchorKey("doc", ""), "");
  return true;
}

TEST(test_build_anchor_key_hash_prefix) {
  std::string key = href_normalization::BuildAnchorKey("doc", "#anchor");
  ASSERT_STR_EQ(key, "doc#anchor");
  return true;
}

TEST(test_normalize_anchor_href_key_normal) {
  std::string key = href_normalization::NormalizeAnchorHrefKey("OEBPS/text/ch.xhtml#section1");
  ASSERT_STR_EQ(key, "OEBPS/text/ch.xhtml#section1");
  return true;
}

TEST(test_normalize_anchor_href_key_no_fragment) {
  ASSERT_STR_EQ(href_normalization::NormalizeAnchorHrefKey("OEBPS/text/ch.xhtml"), "");
  return true;
}

TEST(test_normalize_anchor_href_key_empty) {
  ASSERT_STR_EQ(href_normalization::NormalizeAnchorHrefKey(""), "");
  return true;
}

TEST(test_normalize_anchor_href_key_with_query) {
  std::string key = href_normalization::NormalizeAnchorHrefKey("doc.xhtml#anchor?query=value");
  ASSERT_STR_EQ(key, "doc.xhtml#anchor");
  return true;
}

TEST(test_normalize_anchor_href_key_url_encoded) {
  std::string key = href_normalization::NormalizeAnchorHrefKey("OEBPS/text/ch%2Ehtml%23section");
  // %2E = ., %23 = #, so: "OEBPS/text/ch.html#section" → path="OEBPS/text/ch.html", anchor="section"
  ASSERT_STR_EQ(key, "OEBPS/text/ch.html#section");
  return true;
}

TEST(test_normalize_doc_start_path_strip) {
  std::string key = href_normalization::NormalizeDocStartPathKey("OEBPS/text/ch.xhtml#anchor", true);
  ASSERT_STR_EQ(key, "OEBPS/text/ch.xhtml");
  return true;
}

TEST(test_normalize_doc_start_path_no_strip) {
  std::string key = href_normalization::NormalizeDocStartPathKey("OEBPS/text/ch.xhtml#anchor", false);
  ASSERT_STR_EQ(key, "OEBPS/text/ch.xhtml#anchor");
  return true;
}

TEST(test_normalize_doc_start_path_query) {
  std::string key = href_normalization::NormalizeDocStartPathKey("OEBPS/text/ch.xhtml?query=1#anchor", true);
  ASSERT_STR_EQ(key, "OEBPS/text/ch.xhtml");
  return true;
}

TEST(test_normalize_doc_start_path_empty) {
  ASSERT_STR_EQ(href_normalization::NormalizeDocStartPathKey("", true), "");
  return true;
}

int main() {
  printf("href_normalization tests:\n");

  RUN_TEST(test_basename_root);
  RUN_TEST(test_basename_with_path);
  RUN_TEST(test_basename_trailing_slash);
  RUN_TEST(test_anchor_token_key);
  RUN_TEST(test_anchor_digits);
  RUN_TEST(test_normalize_path_empty);
  RUN_TEST(test_normalize_path_leading_slashes);
  RUN_TEST(test_normalize_path_dotdot);
  RUN_TEST(test_normalize_path_dots);
  RUN_TEST(test_normalize_path_backslashes);
  RUN_TEST(test_normalize_path_already_normalized);
  RUN_TEST(test_url_decode_space);
  RUN_TEST(test_url_decode_hash);
  RUN_TEST(test_url_decode_no_encoding);
  RUN_TEST(test_url_decode_partial);
  RUN_TEST(test_to_lower_ascii);
  RUN_TEST(test_build_anchor_key_normal);
  RUN_TEST(test_build_anchor_key_empty_docpath);
  RUN_TEST(test_build_anchor_key_empty_anchor);
  RUN_TEST(test_build_anchor_key_hash_prefix);
  RUN_TEST(test_normalize_anchor_href_key_normal);
  RUN_TEST(test_normalize_anchor_href_key_no_fragment);
  RUN_TEST(test_normalize_anchor_href_key_empty);
  RUN_TEST(test_normalize_anchor_href_key_with_query);
  RUN_TEST(test_normalize_anchor_href_key_url_encoded);
  RUN_TEST(test_normalize_doc_start_path_strip);
  RUN_TEST(test_normalize_doc_start_path_no_strip);
  RUN_TEST(test_normalize_doc_start_path_query);
  RUN_TEST(test_normalize_doc_start_path_empty);

  printf("\n%d passed, %d failed\n", pass_count, fail_count);
  return fail_count > 0 ? 1 : 0;
}

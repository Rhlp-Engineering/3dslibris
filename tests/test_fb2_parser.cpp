/*
    test_fb2_parser.cpp

    Integration tests for FB2-specific parser branches in book_xml_parser.cpp:
      - section depth tracking (start/end)
      - chapter title capture via <title> inside <section>
      - title chardata accumulation
      - <binary> element handling (start/chardata/end)

    These tests provide the coverage required before extracting FB2 branches
    into book_xml_fb2_handler (Phase 10).
*/

#include "book/book.h"
#include "book/book_context.h"
#include "book/book_parser.h"
#include "shared/app_flow_utils.h"
#include "ui/text.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

#ifndef TEST_FIXTURES_DIR
#define TEST_FIXTURES_DIR "tests/fixtures"
#endif

namespace {

static int g_pass = 0;
static int g_fail = 0;

[[noreturn]] void Fail(const char *label, const char *reason) {
  fprintf(stderr, "FAIL %s: %s\n", label, reason);
  g_fail++;
  std::exit(1);
}

void ExpectTrue(const char *label, bool v) {
  if (!v) Fail(label, "expected true");
  g_pass++;
}

void ExpectFalse(const char *label, bool v) {
  if (v) Fail(label, "expected false");
  g_pass++;
}

void ExpectGt(const char *label, int actual, int threshold) {
  if (actual <= threshold) {
    char buf[128];
    snprintf(buf, sizeof(buf), "expected > %d, got %d", threshold, actual);
    Fail(label, buf);
  }
  g_pass++;
}

void ExpectStrEq(const char *label, const std::string &actual, const char *expected) {
  if (actual != expected) {
    char buf[256];
    snprintf(buf, sizeof(buf), "expected '%s', got '%s'", expected, actual.c_str());
    Fail(label, buf);
  }
  g_pass++;
}

void ExpectEq(const char *label, int actual, int expected) {
  if (actual != expected) {
    char buf[128];
    snprintf(buf, sizeof(buf), "expected %d, got %d", expected, actual);
    Fail(label, buf);
  }
  g_pass++;
}

struct TestCtx {
  Text text;
  BookContext ctx;

  TestCtx() {
    ctx.text = &text;
    ctx.prefs = nullptr;
    ctx.status_reporter = nullptr;
    ctx.paragraph_spacing = nullptr;
    ctx.paragraph_indent = nullptr;
    ctx.orientation = nullptr;
    ctx.draw_background = nullptr;
    ctx.draw_background_user_data = nullptr;
    ctx.draw_top_background = nullptr;
    ctx.draw_top_background_user_data = nullptr;
    ctx.on_spine_progress = nullptr;
    ctx.on_spine_progress_user_data = nullptr;
  }
};

Book *MakeBook(const char *folder, const char *filename) {
  TestCtx *tc = new TestCtx();
  Book *book = new Book(tc->ctx);
  book->SetFolderName(folder);
  book->SetFileName(filename);
  book->format = FORMAT_UNDEF;
  return book;
}

static bool FixtureExists(const char *path) {
  FILE *fp = fopen(path, "r");
  if (!fp) return false;
  fclose(fp);
  return true;
}

// --- tests ---

// basic.fb2 has one section with <title><p>Chapter 1</p></title>.
// Parser must capture it via fb2_title_capture_depth/fb2_title_text path.
void TestFb2SingleChapterTitle() {
  const char *fixture = TEST_FIXTURES_DIR "/books/basic.fb2";
  if (!FixtureExists(fixture)) {
    fprintf(stderr, "SKIP test_fb2_parser single-chapter: fixture not found: %s\n",
            fixture);
    return;
  }

  Book *book = MakeBook(TEST_FIXTURES_DIR "/books", "basic.fb2");
  uint8_t err = book_parser::Open(book);
  ExpectFalse("fb2 single-ch: no error", err != 0);
  ExpectGt("fb2 single-ch: pages > 0", (int)book->GetPageCount(), 0);

  const std::vector<ChapterEntry> &chapters = book->GetChapters();
  ExpectTrue("fb2 single-ch: has chapter", !chapters.empty());
  if (!chapters.empty()) {
    ExpectStrEq("fb2 single-ch: title", chapters[0].title, "Chapter 1");
    ExpectEq("fb2 single-ch: level", (int)chapters[0].level, 0);
  }

  book->Close();
  delete book;
}

// chapters.fb2 has two top-level sections plus one nested section:
//   <section>                          section_depth=1, level=0
//     <title><p>Chapter One</p></title>
//     <section>                        section_depth=2, level=1
//       <title><p>Section 1.1</p></title>
//     </section>
//   </section>
//   <section>                          section_depth=1, level=0
//     <title><p>Chapter Two</p></title>
//   </section>
// After parsing: GetChapters() must have 3 entries with correct titles and levels.
void TestFb2MultiChapterTitles() {
  const char *fixture = TEST_FIXTURES_DIR "/books/chapters.fb2";
  if (!FixtureExists(fixture)) {
    fprintf(stderr, "SKIP test_fb2_parser multi-chapter: fixture not found: %s\n",
            fixture);
    return;
  }

  Book *book = MakeBook(TEST_FIXTURES_DIR "/books", "chapters.fb2");
  uint8_t err = book_parser::Open(book);
  ExpectFalse("fb2 multi-ch: no error", err != 0);
  ExpectGt("fb2 multi-ch: pages > 0", (int)book->GetPageCount(), 0);

  const std::vector<ChapterEntry> &chapters = book->GetChapters();
  ExpectTrue("fb2 multi-ch: at least 3 chapters", chapters.size() >= 3);

  if (chapters.size() >= 1) {
    ExpectStrEq("fb2 multi-ch: ch0 title", chapters[0].title, "Chapter One");
    ExpectEq("fb2 multi-ch: ch0 level", (int)chapters[0].level, 0);
  }
  if (chapters.size() >= 2) {
    ExpectStrEq("fb2 multi-ch: ch1 title", chapters[1].title, "Section 1.1");
    ExpectEq("fb2 multi-ch: ch1 level", (int)chapters[1].level, 1);
  }
  if (chapters.size() >= 3) {
    ExpectStrEq("fb2 multi-ch: ch2 title", chapters[2].title, "Chapter Two");
    ExpectEq("fb2 multi-ch: ch2 level", (int)chapters[2].level, 0);
  }

  book->Close();
  delete book;
}

// chapters.fb2 contains a <binary> element (exercises collecting_fb2_binary
// chardata path and the end-element binary finalization). StoreFb2InlineImage
// is stubbed in the test build, so we just verify no crash and parse completes.
void TestFb2BinaryElementHandled() {
  const char *fixture = TEST_FIXTURES_DIR "/books/chapters.fb2";
  if (!FixtureExists(fixture)) {
    fprintf(stderr, "SKIP test_fb2_parser binary: fixture not found: %s\n",
            fixture);
    return;
  }

  Book *book = MakeBook(TEST_FIXTURES_DIR "/books", "chapters.fb2");
  uint8_t err = book_parser::Open(book);
  ExpectFalse("fb2 binary: no error", err != 0);
  ExpectGt("fb2 binary: pages > 0", (int)book->GetPageCount(), 0);

  book->Close();
  delete book;
}

// Verify section end correctly decrements section depth so that a second
// top-level section gets level=0, not level=1.
// Implicitly tested by TestFb2MultiChapterTitles but explicitly named here.
void TestFb2SectionDepthResets() {
  const char *fixture = TEST_FIXTURES_DIR "/books/chapters.fb2";
  if (!FixtureExists(fixture)) {
    fprintf(stderr, "SKIP test_fb2_parser section-depth: fixture not found\n");
    return;
  }

  Book *book = MakeBook(TEST_FIXTURES_DIR "/books", "chapters.fb2");
  book_parser::Open(book);

  const std::vector<ChapterEntry> &chapters = book->GetChapters();
  if (chapters.size() >= 3) {
    // Third chapter is second top-level section — must be level 0, not 1.
    ExpectEq("fb2 section-depth: ch2 is top-level", (int)chapters[2].level, 0);
    // Nested chapter must be strictly deeper than its parent.
    ExpectTrue("fb2 section-depth: nested > parent",
               chapters[1].level > chapters[0].level);
  }

  book->Close();
  delete book;
}

} // namespace

int main() {
  TestFb2SingleChapterTitle();
  TestFb2MultiChapterTitles();
  TestFb2BinaryElementHandled();
  TestFb2SectionDepthResets();

  fprintf(stderr, "Results: %d/%d passed, %d failed\n", g_pass, g_pass + g_fail,
          g_fail);
  return g_fail > 0 ? 1 : 0;
}

#include "book/book.h"
#include "book/book_context.h"
#include "book/book_parser.h"
#include "shared/app_flow_utils.h"
#include "ui/text.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

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
  if (!v)
    Fail(label, "expected true");
  g_pass++;
}

void ExpectFalse(const char *label, bool v) {
  if (v)
    Fail(label, "expected false");
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
  book->format = FORMAT_EPUB;
  return book;
}

bool HasFixture(const char *path) {
  FILE *fp = fopen(path, "r");
  if (!fp)
    return false;
  fclose(fp);
  return true;
}

void TestBookParserDispatchEpubOpen() {
  const char *fixture = TEST_FIXTURES_DIR "/books/basic.epub";
  if (!HasFixture(fixture)) {
    fprintf(stderr, "SKIP TestBookParserDispatchEpubOpen: fixture not found: %s\n",
            fixture);
    return;
  }

  Book *book = MakeBook(TEST_FIXTURES_DIR "/books", "basic.epub");
  uint8_t err = book_parser::Open(book);
  ExpectFalse("dispatch epub open: no error", err != 0);
  ExpectGt("dispatch epub open: pages > 0", (int)book->GetPageCount(), 0);
  ExpectGt("dispatch epub open: chapters > 0", (int)book->GetChapters().size(), 0);
  book->Close();
  g_pass++;
  delete book;
}

void TestBookParserDispatchEpubReopen() {
  const char *fixture = TEST_FIXTURES_DIR "/books/basic.epub";
  if (!HasFixture(fixture)) {
    fprintf(stderr, "SKIP TestBookParserDispatchEpubReopen: fixture not found: %s\n",
            fixture);
    return;
  }

  Book *book = MakeBook(TEST_FIXTURES_DIR "/books", "basic.epub");
  uint8_t err1 = book_parser::Open(book);
  u16 pages1 = book->GetPageCount();
  u16 chapters1 = (u16)book->GetChapters().size();
  book->Close();

  uint8_t err2 = book_parser::Open(book);
  u16 pages2 = book->GetPageCount();
  u16 chapters2 = (u16)book->GetChapters().size();
  book->Close();
  delete book;

  ExpectFalse("dispatch epub reopen: first open no error", err1 != 0);
  ExpectFalse("dispatch epub reopen: second open no error", err2 != 0);
  ExpectGt("dispatch epub reopen: first pages > 0", (int)pages1, 0);
  ExpectGt("dispatch epub reopen: second pages > 0", (int)pages2, 0);
  ExpectGt("dispatch epub reopen: first chapters > 0", (int)chapters1, 0);
  ExpectGt("dispatch epub reopen: second chapters > 0", (int)chapters2, 0);
}

void TestBookParserDispatchEpubMissingFile() {
  Book *book = MakeBook("/tmp", "nonexistent_3dslibris_dispatch.epub");
  uint8_t err = book_parser::Open(book);
  ExpectTrue("dispatch epub missing: returns error", err != 0);
  book->Close();
  g_pass++;
  delete book;
}

} // namespace

int main() {
  TestBookParserDispatchEpubOpen();
  TestBookParserDispatchEpubReopen();
  TestBookParserDispatchEpubMissingFile();

  fprintf(stderr, "Results: %d/%d passed, %d failed\n", g_pass, g_pass + g_fail,
          g_fail);
  return g_fail > 0 ? 1 : 0;
}

#include "formats/fb2/fb2_parser.h"

#include "book/book.h"
#include "formats/fb2/fb2.h"
#include "formats/common/page_text_extract_utils.h"
#include "formats/common/plain_parser.h"
#include "formats/common/xml_book_parser.h"

#include <vector>

namespace {

static void BuildFb2FallbackChapters(Book *book) {
  if (!book)
    return;
  if (!book->GetChapters().empty())
    return;
  if (book->GetPageCount() == 0)
    return;

  std::vector<std::string> lines;
  std::vector<u16> line_pages;
  for (u16 page = 0; page < book->GetPageCount(); page++) {
    const std::vector<std::string> page_lines =
        page_text_extract_utils::ExtractTextLinesFromPage(book->GetPage(page));
    for (size_t i = 0; i < page_lines.size(); i++) {
      lines.push_back(page_lines[i]);
      line_pages.push_back(page);
    }
  }

  if (lines.empty())
    return;

  bool prev_blank = true;
  bool prev_candidate = false;
  for (size_t i = 0; i < lines.size(); i++) {
    bool curr_blank = plain_parser::IsBlankLine(lines[i]);
    bool next_blank =
        (i + 1 >= lines.size()) || plain_parser::IsBlankLine(lines[i + 1]);

    bool curr_strong = false;
    bool curr_candidate =
        plain_parser::LooksLikePlainChapterHeading(lines[i], &curr_strong);
    bool next_strong = false;
    bool next_candidate =
        (i + 1 < lines.size()) &&
        plain_parser::LooksLikePlainChapterHeading(lines[i + 1], &next_strong);

    if (curr_candidate && plain_parser::ShouldAcceptHeuristicHeading(
                              lines[i], prev_blank, next_blank, prev_candidate,
                              next_candidate, curr_strong))
      plain_parser::AddChapterAtPageIfUnique(book, line_pages[i], lines[i], 0);

    prev_blank = curr_blank;
    prev_candidate = curr_candidate;
  }
}

} // namespace

namespace fb2_parser {

uint8_t Parse(Book *book, const char *path, bool fulltext,
              const BookParseDeps &deps) {
  return xml_book_parser::ParseXmlBookFile(
      book, path, fulltext, deps, BuildFb2FallbackChapters,
      plain_parser::SetNonEpubTocConfidence);
}

int ExtractCover(Book *book, const std::string &path) {
  return fb2_extract_cover(book, path);
}

} // namespace fb2_parser

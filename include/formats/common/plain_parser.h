#pragma once

#include <string>
#include <vector>

#include <3ds/types.h>

#include "book/book_parse_deps.h"
#include "formats/common/plain_text_perf_utils.h"
#include "formats/common/plain_text_stream.h"

namespace plain_parser {

std::string TrimAsciiWhitespace(const std::string &in);
std::string CollapseAsciiWhitespace(const std::string &in);
std::string FoldLatinForMatch(const std::string &in);
int CountAsciiWords(const std::string &line);
bool LooksLikePlainChapterHeading(const std::string &line, bool *strong_signal_out);
bool IsBlankLine(const std::string &line);
bool ShouldAcceptHeuristicHeading(const std::string &line, bool prev_blank,
                                  bool next_blank, bool prev_candidate,
                                  bool next_candidate, bool strong_signal);
bool IsMostlyDigitsOrPunctuation(const std::string &s);

void AddChapterAtPageIfUnique(Book *book, u16 page, const std::string &title,
                              u8 level);
void AddChapterIfUnique(Book *book, const std::string &title, u8 level);
void SetNonEpubTocConfidence(Book *book, bool strong);

typedef plain_text_stream::State State;

bool InitState(Book *book, const std::string &text_utf8,
               const BookParseDeps &deps, bool detect_heuristic_headings,
               State *out);

bool ContinueState(State *state, const std::string &text_utf8, u32 budget_ms,
                   u16 page_budget, u16 min_pages_before_stop,
                   std::vector<u32> *text_cursor_per_page,
                   plain_text_perf_utils::PlainTextStreamPerf *perf_out);

plain_text_stream::ContinueCallbacks MakeContinueCallbacks();

u8 ParseBuffer(Book *book, const std::string &text_utf8,
               bool detect_heuristic_headings = true);
u8 ParseTxtFile(Book *book, const char *path);
u8 ParseRtfFile(Book *book, const char *path);
void BuildFb2FallbackChapters(Book *book);

} // namespace plain_parser

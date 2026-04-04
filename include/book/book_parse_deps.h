#pragma once

#include <string>

#include "book/book.h"
#include "ui/text.h"

struct BookParseDeps {
  IStatusReporter *reporter;
  Text *ts;
  Prefs *prefs;
  std::string regular_font_path;
  int paragraph_spacing;
  int paragraph_indent;
  int orientation;

  BookParseDeps()
      : reporter(NULL), ts(NULL), prefs(NULL), regular_font_path(""), paragraph_spacing(0),
        paragraph_indent(0), orientation(0) {}
};

inline BookParseDeps BuildBookParseDeps(Book *book) {
  BookParseDeps deps;
  deps.reporter = book ? book->GetStatusReporter() : NULL;
  deps.ts = book ? book->GetText() : NULL;
  deps.prefs = book ? book->GetPrefs() : NULL;
  deps.regular_font_path =
      book && book->GetText()
          ? book->GetText()->GetFontFile(TEXT_STYLE_REGULAR)
          : "";
  deps.paragraph_spacing = book ? book->GetParagraphSpacing() : 0;
  deps.paragraph_indent = book ? book->GetParagraphIndent() : 0;
  deps.orientation = book ? book->GetOrientation() : 0;
  return deps;
}

#pragma once

#include "book/book.h"

struct BookParseDeps {
  IStatusReporter *reporter;
  Text *ts;
  Prefs *prefs;
  int paragraph_spacing;
  int paragraph_indent;
  int orientation;

  BookParseDeps()
      : reporter(NULL), ts(NULL), prefs(NULL), paragraph_spacing(0),
        paragraph_indent(0), orientation(0) {}
};

inline BookParseDeps BuildBookParseDeps(Book *book) {
  BookParseDeps deps;
  deps.reporter = book ? book->GetStatusReporter() : NULL;
  deps.ts = book ? book->GetText() : NULL;
  deps.prefs = book ? book->GetPrefs() : NULL;
  deps.paragraph_spacing = book ? book->GetParagraphSpacing() : 0;
  deps.paragraph_indent = book ? book->GetParagraphIndent() : 0;
  deps.orientation = book ? book->GetOrientation() : 0;
  return deps;
}

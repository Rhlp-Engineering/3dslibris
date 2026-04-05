#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

namespace text_bidi_utils {

struct BidiRun {
  size_t start;       // índice en array de codepoints
  size_t length;
  int bidi_level;     // 0=LTR, odd=RTL
};

// Fast check: ¿hay algún codepoint RTL en el rango?
// RTL = Hebrew (U+0590-U+05FF), Arabic (U+0600-U+06FF), etc.
bool ContainsRTL(const uint32_t *codepoints, size_t count);

// Analiza codepoints y produce runs direccionales usando fz_bidi_fragment_text de MuPDF.
// Devuelve false si no hay contexto MuPDF disponible (fallback: un solo run LTR).
bool AnalyzeBidiRuns(const uint32_t *codepoints, size_t count,
                     std::vector<BidiRun> *runs);

// Reordena glyphs dentro de una línea según UAX#9 L2.
// Los codepoints entran al page buffer en orden visual.
void ReorderLineForDisplay(std::vector<uint32_t> &codepoints,
                           size_t line_start, size_t line_end,
                           const std::vector<BidiRun> &runs);

} // namespace text_bidi_utils

set -eu

ROOT="$(CDPATH= cd -- "$(dirname "$0")/.." && pwd)"
OUTDIR="${TMPDIR:-/tmp}/3dslibris-tests"
mkdir -p "$OUTDIR"

c++ -std=c++11 \
  "$ROOT/tests/test_glyph_cache_lru.cpp" \
  -I"$ROOT/include" \
  -o "$OUTDIR/test_glyph_cache_lru"

"$OUTDIR/test_glyph_cache_lru"

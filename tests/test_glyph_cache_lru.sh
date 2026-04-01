set -eu
source "$(dirname "$0")/test_build.sh"
build_test test_glyph_cache_lru \
  "$TEST_ROOT/tests/test_glyph_cache_lru.cpp"

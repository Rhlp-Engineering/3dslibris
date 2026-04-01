set -eu
source "$(dirname "$0")/test_build.sh"
build_test test_mobi_cover_meta_cache \
  "$TEST_ROOT/tests/test_mobi_cover_meta_cache.cpp" \
  "$TEST_ROOT/source/formats/mobi/mobi_cover_meta_cache.cpp"

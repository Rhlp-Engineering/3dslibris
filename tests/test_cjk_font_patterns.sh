set -eu
source "$(dirname "$0")/test_build.sh"
build_test test_cjk_font_patterns \
  "$TEST_ROOT/tests/test_cjk_font_patterns.cpp"

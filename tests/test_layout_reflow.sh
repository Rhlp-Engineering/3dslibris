set -eu
source "$(dirname "$0")/test_build.sh"
build_test test_layout_reflow \
  "$TEST_ROOT/tests/test_layout_reflow.cpp" \
  "$TEST_ROOT/source/book/layout_reflow.cpp" \
  -I"$TEST_ROOT/source/book"

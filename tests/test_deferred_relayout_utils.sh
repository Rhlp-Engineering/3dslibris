set -eu
source "$(dirname "$0")/test_build.sh"
build_test test_deferred_relayout_utils \
  "$TEST_ROOT/tests/test_deferred_relayout_utils.cpp" \
  "$TEST_ROOT/source/book/layout_reflow.cpp" \
  -I"$TEST_ROOT/source/book"

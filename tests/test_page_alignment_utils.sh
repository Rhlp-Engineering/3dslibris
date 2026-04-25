set -eu
source "$(dirname "$0")/test_build.sh"
build_test test_page_alignment_utils \
  "$TEST_ROOT/tests/test_page_alignment_utils.cpp" \
  "$TEST_ROOT/source/book/page_alignment_utils.cpp"

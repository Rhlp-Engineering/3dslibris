set -eu
source "$(dirname "$0")/test_build.sh"
build_test test_page_buffer_utils \
  "$TEST_ROOT/tests/test_page_buffer_utils.cpp"

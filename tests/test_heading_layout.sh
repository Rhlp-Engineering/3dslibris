set -eu
source "$(dirname "$0")/test_build.sh"
build_test test_heading_layout \
  "$TEST_ROOT/tests/test_heading_layout.cpp" \
  "$TEST_ROOT/source/book/heading_layout.cpp"

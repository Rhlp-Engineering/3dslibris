set -eu
source "$(dirname "$0")/test_build.sh"
build_test test_inline_image_layout \
  "$TEST_ROOT/tests/test_inline_image_layout.cpp" \
  "$TEST_ROOT/source/book/inline_image_layout.cpp"

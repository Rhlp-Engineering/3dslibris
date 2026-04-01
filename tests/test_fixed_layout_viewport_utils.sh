set -eu
source "$(dirname "$0")/test_build.sh"
build_test test_fixed_layout_viewport_utils \
  "$TEST_ROOT/tests/test_fixed_layout_viewport_utils.cpp"

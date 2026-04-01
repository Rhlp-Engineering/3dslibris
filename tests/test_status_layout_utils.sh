set -eu
source "$(dirname "$0")/test_build.sh"
build_test test_status_layout_utils \
  "$TEST_ROOT/tests/test_status_layout_utils.cpp"

set -eu
source "$(dirname "$0")/test_build.sh"
build_test test_utf8_utils \
  "$TEST_ROOT/tests/test_utf8_utils.cpp" \
  "$TEST_ROOT/source/shared/utf8_utils.cpp"

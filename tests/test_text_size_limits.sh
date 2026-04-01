set -eu
source "$(dirname "$0")/test_build.sh"
build_test test_text_size_limits \
  "$TEST_ROOT/tests/test_text_size_limits.cpp"

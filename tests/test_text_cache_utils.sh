set -eu
source "$(dirname "$0")/test_build.sh"
build_test test_text_cache_utils \
  "$TEST_ROOT/tests/test_text_cache_utils.cpp"

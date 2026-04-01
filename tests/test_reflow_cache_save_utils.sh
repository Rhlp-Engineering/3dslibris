set -eu
source "$(dirname "$0")/test_build.sh"
build_test test_reflow_cache_save_utils \
  "$TEST_ROOT/tests/test_reflow_cache_save_utils.cpp"

set -eu
source "$(dirname "$0")/test_build.sh"
build_test test_browser_warmup_utils \
  "$TEST_ROOT/tests/test_browser_warmup_utils.cpp"

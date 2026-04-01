set -eu
source "$(dirname "$0")/test_build.sh"
build_test test_browser_nav \
  "$TEST_ROOT/tests/test_browser_nav.cpp" \
  "$TEST_ROOT/source/ui/browser_nav.cpp"

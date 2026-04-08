set -eu
source "$(dirname "$0")/test_build.sh"
build_test test_bug_regressions \
  "$TEST_ROOT/tests/test_bug_regressions.cpp"

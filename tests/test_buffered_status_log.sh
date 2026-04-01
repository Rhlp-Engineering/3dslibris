set -eu
source "$(dirname "$0")/test_build.sh"
build_test test_buffered_status_log \
  "$TEST_ROOT/tests/test_buffered_status_log.cpp" \
  "$TEST_ROOT/source/formats/common/buffered_status_log.cpp"

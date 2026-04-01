set -eu
source "$(dirname "$0")/test_build.sh"
build_test test_file_read_utils \
  "$TEST_ROOT/tests/test_file_read_utils.cpp" \
  "$TEST_ROOT/source/formats/common/file_read_utils.cpp"

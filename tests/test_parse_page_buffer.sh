set -eu
source "$(dirname "$0")/test_build.sh"
build_test test_parse_page_buffer \
  "$TEST_ROOT/tests/test_parse_page_buffer.cpp" \
  "$TEST_ROOT/source/core/parse.cpp" \
  "-I$TEST_ROOT/tests/stubs"

set -eu
source "$(dirname "$0")/test_build.sh"
build_test test_book_error \
  "$TEST_ROOT/tests/test_book_error.cpp"

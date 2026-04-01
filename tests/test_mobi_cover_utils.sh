set -eu
source "$(dirname "$0")/test_build.sh"
build_test test_mobi_cover_utils \
  "$TEST_ROOT/tests/test_mobi_cover_utils.cpp"

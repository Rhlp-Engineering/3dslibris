set -eu
source "$(dirname "$0")/test_build.sh"
build_test test_mobi_cache_utils \
  "$TEST_ROOT/tests/test_mobi_cache_utils.cpp"

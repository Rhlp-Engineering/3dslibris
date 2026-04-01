set -eu
source "$(dirname "$0")/test_build.sh"
build_test test_mobi_position_map \
  "$TEST_ROOT/tests/test_mobi_position_map.cpp" \
  "$TEST_ROOT/source/formats/mobi/mobi_position_map.cpp"

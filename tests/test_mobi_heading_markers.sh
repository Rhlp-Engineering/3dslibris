set -eu
source "$(dirname "$0")/test_build.sh"
build_test test_mobi_heading_markers \
  "$TEST_ROOT/tests/test_mobi_heading_markers.cpp" \
  "$TEST_ROOT/source/formats/mobi/mobi_heading_markers.cpp"

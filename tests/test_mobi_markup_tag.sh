set -eu
source "$(dirname "$0")/test_build.sh"
build_test test_mobi_markup_tag \
  "$TEST_ROOT/tests/test_mobi_markup_tag.cpp" \
  "$TEST_ROOT/source/formats/mobi/mobi_markup_tag.cpp"

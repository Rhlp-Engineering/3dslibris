set -eu
source "$(dirname "$0")/test_build.sh"
build_test test_mobi_record_decode \
  "$TEST_ROOT/tests/test_mobi_record_decode.cpp" \
  "$TEST_ROOT/source/formats/mobi/mobi_record_decode.cpp"

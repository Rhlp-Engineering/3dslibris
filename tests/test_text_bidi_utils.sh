set -eu
source "$(dirname "$0")/test_build.sh"
build_test test_text_bidi_utils \
  "$TEST_ROOT/tests/test_text_bidi_utils.cpp" \
  "$TEST_ROOT/source/shared/text_bidi_utils.cpp" \
  "$TEST_ROOT/tests/stubs/mupdf_bidi_stub.cpp" \
  -I"$TEST_ROOT/tests/stubs"

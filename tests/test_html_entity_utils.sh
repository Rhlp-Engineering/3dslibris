set -eu
source "$(dirname "$0")/test_build.sh"
build_test test_html_entity_utils \
  "$TEST_ROOT/tests/test_html_entity_utils.cpp" \
  "$TEST_ROOT/source/formats/common/html_entity_utils.cpp" \
  "$TEST_ROOT/source/formats/common/text_helpers.cpp" \
  "$TEST_ROOT/source/shared/utf8_utils.cpp"

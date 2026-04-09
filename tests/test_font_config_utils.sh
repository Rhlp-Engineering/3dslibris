set -eu
source "$(dirname "$0")/test_build.sh"
build_test test_font_config_utils \
  "$TEST_ROOT/tests/test_font_config_utils.cpp" \
  "$TEST_ROOT/source/settings/font_config_utils.cpp" \
  -I"$TEST_ROOT/tests/stubs"

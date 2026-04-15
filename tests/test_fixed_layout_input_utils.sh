set -eu

. "$(CDPATH= cd -- "$(dirname "$0")" && pwd)/test_build.sh"

build_test test_fixed_layout_input_utils \
  "$TEST_ROOT/tests/test_fixed_layout_input_utils.cpp" \
  "$TEST_ROOT/source/reader/fixed_layout_input_utils.cpp"

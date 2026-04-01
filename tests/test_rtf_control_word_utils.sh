set -eu
source "$(dirname "$0")/test_build.sh"
build_test test_rtf_control_word_utils \
  "$TEST_ROOT/tests/test_rtf_control_word_utils.cpp"

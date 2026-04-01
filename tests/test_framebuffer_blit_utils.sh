set -eu
source "$(dirname "$0")/test_build.sh"
build_test test_framebuffer_blit_utils \
  "$TEST_ROOT/tests/test_framebuffer_blit_utils.cpp"

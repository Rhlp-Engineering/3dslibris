set -eu
source "$(dirname "$0")/test_build.sh"
build_test test_app_flow_utils \
  "$TEST_ROOT/tests/test_app_flow_utils.cpp" \
  "$TEST_ROOT/source/shared/app_flow_utils.cpp"

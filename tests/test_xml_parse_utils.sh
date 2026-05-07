set -eu
source "$(dirname "$0")/test_build.sh"
build_test test_xml_parse_utils \
  "$TEST_ROOT/tests/test_xml_parse_utils.cpp" \
  "$TEST_ROOT/source/formats/common/xml_parse_utils.cpp" \
  --stubs minizip_unzip_stubs.cpp \
  --expat

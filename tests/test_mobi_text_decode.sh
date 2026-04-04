#!/bin/bash
source "$(dirname "$0")/test_build.sh"
build_test test_mobi_text_decode \
  tests/test_mobi_text_decode.cpp \
  tests/stubs/mobi_text_decode_deps.cpp \
  -I"$TEST_ROOT/tests/stubs"

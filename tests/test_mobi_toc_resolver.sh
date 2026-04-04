#!/bin/bash
source "$(dirname "$0")/test_build.sh"
build_test test_mobi_toc_resolver \
  tests/test_mobi_toc_resolver.cpp \
  source/formats/mobi/mobi_toc_resolver.cpp \
  source/formats/mobi/mobi_toc_finalize.cpp \
  source/formats/mobi/mobi_toc_apply.cpp \
  -I"$TEST_ROOT/tests/stubs"

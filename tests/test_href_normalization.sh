#!/bin/bash
source "$(dirname "$0")/test_build.sh"
build_test test_href_normalization \
  tests/test_href_normalization.cpp \
  source/formats/common/href_normalization.cpp

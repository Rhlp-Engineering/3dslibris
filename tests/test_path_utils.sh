#!/bin/bash
source "$(dirname "$0")/test_build.sh"
build_test test_path_utils \
  tests/test_path_utils.cpp

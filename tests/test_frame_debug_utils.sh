#!/bin/sh
set -eu
cd "$(dirname "$0")/.."
. ./tests/test_build.sh

build_test test_frame_debug_utils \
  tests/test_frame_debug_utils.cpp

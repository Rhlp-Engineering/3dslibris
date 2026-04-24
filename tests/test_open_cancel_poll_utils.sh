#!/bin/sh
set -eu
cd "$(dirname "$0")/.."
. ./tests/test_build.sh

build_test test_open_cancel_poll_utils \
  tests/test_open_cancel_poll_utils.cpp \
  source/shared/open_cancel_poll_utils.cpp

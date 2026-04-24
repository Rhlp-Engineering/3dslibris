#!/bin/sh
set -eu
cd "$(dirname "$0")/.."
. ./tests/test_build.sh

build_test test_reader_suspend_policy_utils \
  tests/test_reader_suspend_policy_utils.cpp \
  source/reader/suspend_policy_utils.cpp

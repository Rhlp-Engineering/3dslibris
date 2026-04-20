#!/bin/sh
set -eu
cd "$(dirname "$0")/.."
. ./tests/test_build.sh

build_test test_reflow_open_gate \
  tests/test_reflow_open_gate.cpp \
  source/reader/reflow_open_gate_utils.cpp

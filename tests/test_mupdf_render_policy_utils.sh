#!/usr/bin/env bash
set -eu

source "$(dirname "$0")/test_build.sh"

build_test test_mupdf_render_policy_utils \
  "$TEST_ROOT/tests/test_mupdf_render_policy_utils.cpp"

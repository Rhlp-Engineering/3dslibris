#!/bin/sh
set -eu

cd "$(dirname "$0")/.."

mkdir -p build/tests

c++ -std=c++17 -Wall -Wextra -Iinclude \
  source/core/inline_image_flow.cpp \
  tests/test_inline_image_flow.cpp \
  -o build/tests/test_inline_image_flow

build/tests/test_inline_image_flow

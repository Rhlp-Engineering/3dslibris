#!/bin/sh
set -eu

cd "$(dirname "$0")/.."

g++ -std=c++17 -Iinclude tests/test_fixed_layout_viewport_utils.cpp -o /tmp/test_fixed_layout_viewport_utils
/tmp/test_fixed_layout_viewport_utils

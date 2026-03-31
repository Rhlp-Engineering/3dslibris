#!/bin/sh
set -eu

cd "$(dirname "$0")/.."

g++ -std=c++17 -Iinclude tests/test_status_layout_utils.cpp -o /tmp/test_status_layout_utils
/tmp/test_status_layout_utils

#!/bin/sh
set -eu

cd "$(dirname "$0")/.."

g++ -std=c++17 -Iinclude tests/test_text_buffer_utils.cpp -o /tmp/test_text_buffer_utils
/tmp/test_text_buffer_utils

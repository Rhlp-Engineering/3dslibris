#!/bin/sh
set -eu

cd "$(dirname "$0")/.."

g++ -std=c++17 -Iinclude tests/test_rtf_control_word_utils.cpp -o /tmp/test_rtf_control_word_utils
/tmp/test_rtf_control_word_utils

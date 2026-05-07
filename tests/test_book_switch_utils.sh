#!/bin/sh
set -eu
cd "$(dirname "$0")/.."
mkdir -p build/test
"${CXX:-c++}" -std=c++17 ${CXXFLAGS:-} -Iinclude tests/test_book_switch_utils.cpp ${LDFLAGS:-} -o build/test/test_book_switch_utils
./build/test/test_book_switch_utils

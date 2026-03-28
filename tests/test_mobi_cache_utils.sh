#!/bin/sh
set -eu
cd "$(dirname "$0")/.."
clang++ -std=c++11 -Iinclude tests/test_mobi_cache_utils.cpp -o /tmp/test_mobi_cache_utils
/tmp/test_mobi_cache_utils

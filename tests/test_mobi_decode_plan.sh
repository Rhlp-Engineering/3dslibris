#!/bin/sh
set -eu
cd "$(dirname "$0")/.."
"${CXX:-c++}" -std=c++11 ${CXXFLAGS:-} -Iinclude tests/test_mobi_decode_plan.cpp ${LDFLAGS:-} -o /tmp/test_mobi_decode_plan
/tmp/test_mobi_decode_plan

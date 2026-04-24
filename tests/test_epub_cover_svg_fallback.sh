#!/bin/sh
set -eu
cd "$(dirname "$0")/.."
. ./tests/test_build.sh

build_test test_epub_cover_svg_fallback \
  tests/test_epub_cover_svg_fallback.cpp

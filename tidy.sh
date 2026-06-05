#!/bin/bash

CHECKS='-*,clang-analyzer-*,performance-*,bugprone-*'
FILES=$(git ls-files '*.cpp' '*.cxx' '*.cc' 2>/dev/null | tr '\n' ' ')

if [[ -z "$FILES" ]]; then
  FILES=$(find . -maxdepth 1 \( -name '*.cpp' -o -name '*.cxx' -o -name '*.cc' \) -printf '%P ')
fi

if [[ -z "$FILES" ]]; then
  echo "No source files to analyze."
  exit 0
fi

set -eo pipefail

for f in $FILES; do
  echo "Running clang-tidy on $f"
  clang-tidy \
    -checks="$CHECKS" \
    "$f" \
    -- \
    -std=c++17 \
    -I../external/eigen \
    -I/usr/include/eigen3 \
    -I../external/lazycsv/include \
    -I/usr/include/x86_64-linux-gnu/openblas-pthread/
done

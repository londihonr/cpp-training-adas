#!/bin/bash
set -e
cd "$(dirname "$0")/.."

mkdir -p build
cd build
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug ..
cmake --build . -- -j"$(nproc 2>/dev/null || echo 2)"

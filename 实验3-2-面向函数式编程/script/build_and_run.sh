#!/bin/bash
set -e
bash "$(dirname "$0")/build.sh"
cd "$(dirname "$0")/.."
./build/tests/training_main

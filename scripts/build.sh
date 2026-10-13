#!/bin/bash
set -e
cd "$(dirname "$0")/.."
make clean
make
echo "→ out/Astral.dylib"
ls -lh out/Astral.dylib

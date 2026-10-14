#!/bin/sh
# Zip the mountable test images from testing/r2r as <platform>/<file>.
# Usage: pack-test-images.sh <output.zip>
set -e

out=$(realpath -m "$1")
cd testing/r2r

# Image extensions per platform, plus the Adam .ddp boot loaders.
EXTS="atr po dsk img imd ddp prg lnx cas ptp mrm abs COM"
set -- -name '*_BOOTSTRAP.bin'
for ext in $EXTS; do
  set -- "$@" -o -name "*.$ext"
done

find . -type f \( "$@" \) | sed 's|^\./||' | sort | zip -q "$out" -@

unzip -l "$out"

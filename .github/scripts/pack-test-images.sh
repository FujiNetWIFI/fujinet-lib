#!/bin/sh
# Zip the mountable test images from testing/r2r as <platform>/<file>.
# Usage: pack-test-images.sh <output.zip>
set -e

out=$(realpath -m "$1")
cd testing/r2r

find . -type f \( \
  -name '*.atr' -o -name '*.po' -o -name '*.dsk' -o -name '*.img' \
  -o -name '*.imd' -o -name '*.ddp' -o -name '*_BOOTSTRAP.bin' \
  -o -name '*.prg' -o -name '*.lnx' -o -name '*.cas' -o -name '*.ptp' \
  -o -name '*.mrm' -o -name '*.abs' -o -name '*.COM' \) \
  | sed 's|^\./||' | sort | zip -q "$out" -@

unzip -l "$out"

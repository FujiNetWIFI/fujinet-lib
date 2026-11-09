#!/bin/sh
# Run an auto-run cart headless in jzIntv against a FujiNet and print the
# screen. Usage: tools/run.sh <program> [host:port]
# Needs a FujiNet that is not the one anyone is using: the tests rewrite
# its host slots, keys and WiFi settings.
set -u
PROG=${1:?program name, e.g. appkey}
FUJINET=${2:-localhost:9975}
JZINTV=${JZINTV:-$HOME/Workspace/jzintv-20200712-src/bin/linux/jzintv}
BIOS=${BIOS:-$HOME/Workspace/jzintv-20200712-src/rom}
LIMIT=${LIMIT:-900}
HERE=$(cd "$(dirname "$0")/.." && pwd)
NAME=fnlib-test-$PROG
OUT=${OUT:-$(mktemp -d)}

${MAKE_CMD:-make} -C "$HERE" PROG="$PROG" >/dev/null || exit 2
ADDR=$(awk '/label_RESULTS_READY/ {print $1}' "$HERE/build/$NAME.sym" | sed 's/^0*//')

printf 'b %s\nr\nm 9b00 10\nm 200 f0\nq\n' "$ADDR" > "$OUT/script.txt"
cd "$OUT" || exit 2
SDL_VIDEODRIVER=dummy SDL_AUDIODRIVER=dummy timeout "$LIMIT" "$JZINTV" -d \
  --script="$OUT/script.txt" -a0 -e "$BIOS/exec.bin" -g "$BIOS/grom.bin" \
  --fujinet="$FUJINET" "$HERE/build/$NAME" > "$OUT/jz.log" 2>&1
RC=$?
python3 -I "$HERE/tools/results.py" "$OUT/jz.log"; RC2=$?
[ $RC -eq 0 ] || echo "jzintv exit $RC (log: $OUT/jz.log)" >&2
exit $RC2


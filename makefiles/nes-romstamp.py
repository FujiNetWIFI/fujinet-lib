#!/usr/bin/env python3
"""nes-romstamp.py -- validate (and stamp) a built NES FujiNet client.

Ported from fujinet-firmware/pico/nes/tools/checkrom.py, the upstream source
of the layout contract:

  - an iNES header, and a file exactly as long as the header promises;
  - the "FUJI" claim in the last 16 bytes of PRG ($FFF0), which is what keeps
    the cartridge's mailbox alive after it boots the image. With --stamp it is
    written first (the linker config reserves the space);
  - a reset vector inside $8000-$FFFF;
  - no read-modify-write instruction targeting the mailbox's write-only pages
    ($5500-$57FF): the 6502 writes the old value back first and that lands as
    a spurious event. With --map the scan walks only the code segments the
    ld65 map (-m) lists, so strings and tables cannot masquerade as opcodes;
    without one the whole PRG is scanned as code, which is safe but noisy.

A failing image is deleted, so an image left behind with a fresh mtime cannot
look up to date.

Usage: nes-romstamp.py [--stamp] [--map file.map] image.nes [image2.nes ...]
"""

import os
import re
import sys

CLAIM = b"FUJI"
WR_LO, WR_HI = 0x5500, 0x57FF
RMW = {0x0E, 0x1E, 0x2E, 0x3E, 0x4E, 0x5E, 0x6E, 0x7E, 0xCE, 0xDE, 0xEE, 0xFE,
       0x0F, 0x1F, 0x2F, 0x3F, 0x4F, 0x5F, 0x6F, 0x7F, 0xCF, 0xDF, 0xEF, 0xFF}

LEN = [1] * 256
for op in (0x69, 0x29, 0xC9, 0xE0, 0xC0, 0x49, 0xA9, 0xA2, 0xA0, 0x09, 0xE9,
           0xA5, 0xA6, 0xA4, 0x85, 0x86, 0x84, 0x65, 0x25, 0x06, 0x24, 0xC5,
           0xC6, 0x45, 0xE6, 0x46, 0x26, 0x66, 0xE5, 0x05, 0x75, 0x35, 0x16,
           0xD5, 0xD6, 0x55, 0xF6, 0x56, 0x36, 0x76, 0xF5, 0x15, 0xB5, 0xB4,
           0x95, 0x94, 0xB6, 0x96, 0x61, 0x21, 0xC1, 0x41, 0xA1, 0x01, 0xE1,
           0x81, 0x71, 0x31, 0xD1, 0x51, 0xB1, 0x11, 0xF1, 0x91,
           0x10, 0x30, 0x50, 0x70, 0x90, 0xB0, 0xD0, 0xF0):
    LEN[op] = 2
for op in (0x6D, 0x2D, 0x0E, 0x2C, 0xCD, 0xEC, 0xCC, 0xCE, 0x4D, 0xEE, 0x4C,
           0x20, 0xAD, 0xAE, 0xAC, 0x4E, 0x0D, 0x2E, 0x6E, 0xED, 0x8D, 0x8E,
           0x8C, 0x7D, 0x3D, 0x1E, 0xDD, 0xDE, 0x5D, 0xFD, 0xFE, 0x5E, 0xBD,
           0xBC, 0x3E, 0x7E, 0x1D, 0x9D, 0x79, 0x39, 0xD9, 0x59, 0xB9, 0xBE,
           0x19, 0xF9, 0x99, 0x6C):
    LEN[op] = 3


CODE_SEGMENTS = ("STARTUP", "LOWCODE", "ONCE", "CODE")


def code_ranges(mapfile):
    """(start, end) console-address ranges of the code segments, from ld65 -m."""
    if not mapfile:
        return None
    ranges = []
    for name, start, end in re.findall(r"^(\w+)\s+([0-9A-F]{6})\s+([0-9A-F]{6})\s+[0-9A-F]{6}",
                                       open(mapfile).read(), re.M):
        if name in CODE_SEGMENTS:
            ranges.append((int(start, 16), int(end, 16)))
    return ranges


def sizes(img):
    prg = img[4] * 16384
    chr_ = img[5] * 8192
    if (img[7] & 0x0C) == 0x08:
        prg = ((img[9] & 0x0F) << 8 | img[4]) * 16384
        chr_ = ((img[9] >> 4) << 8 | img[5]) * 8192
    return prg, chr_


def stamp(path):
    with open(path, "r+b") as f:
        img = f.read(16)
        prg, _ = sizes(img)
        f.seek(16 + prg - 16)
        f.write(CLAIM)


def check(path, mapfile=None):
    with open(path, "rb") as f:
        img = f.read()
    if len(img) < 16 or img[:4] != b"NES\x1a":
        return ["no iNES header"]
    prg, chr_ = sizes(img)
    problems = []
    if len(img) != 16 + prg + chr_:
        problems.append("file is %d bytes, header promises %d" % (len(img), 16 + prg + chr_))
        return problems
    p = img[16:16 + prg]
    if p[-16:-12] != CLAIM:
        problems.append("claim signature 'FUJI' missing at PRG end-16 ($FFF0)")
    vec = p[-4] | (p[-3] << 8)
    if vec < 0x8000:
        problems.append("reset vector $%04X is not in $8000-$FFFF" % vec)
    # A 32K image is mapped at $8000; the code ranges from the map are console
    # addresses, so they are scanned relative to that.
    base = 0x10000 - len(p) if len(p) <= 0x8000 else 0x8000
    ranges = code_ranges(mapfile) or [(base, base + len(p) - 1)]
    for start, end in ranges:
        pc = start - base
        stop = min(end - base, len(p) - 1)
        while pc < stop - 1:
            op = p[pc]
            n = LEN[op]
            if n == 3 and op in RMW:
                tgt = p[pc + 1] | (p[pc + 2] << 8)
                if WR_LO <= tgt <= WR_HI:
                    problems.append("RMW opcode $%02X at $%04X targets the write-only page $%04X"
                                    % (op, base + pc, tgt))
            pc += n
    return problems


def main():
    args = sys.argv[1:]
    do_stamp = False
    mapfile = None
    while args and args[0].startswith("--"):
        if args[0] == "--stamp":
            do_stamp = True
            args = args[1:]
        elif args[0] == "--map" and len(args) > 1:
            mapfile = args[1]
            args = args[2:]
        else:
            break
    if not args:
        print(__doc__.strip(), file=sys.stderr)
        return 2
    rc = 0
    for path in args:
        if do_stamp and os.path.getsize(path) > 32:
            stamp(path)
        problems = check(path, mapfile)
        if problems:
            rc = 1
            for p in problems:
                print("%s: %s" % (path, p), file=sys.stderr)
            os.remove(path)
        else:
            print("%s: ok" % path)
    return rc


if __name__ == "__main__":
    sys.exit(main())

#!/usr/bin/env python3
"""Decode a jzIntv debugger log written by tools/run.sh.

Prints the 20x12 screen as text, with the colour of each row's text
(g green, r red, y yellow, w white), and the result block. Exits 0 when
the run finished with no failed check, 1 otherwise.
"""
import re
import sys

COLOUR = {0: "k", 1: "b", 2: "r", 3: "t", 4: "G", 5: "g", 6: "y", 7: "w"}


def dump(lines, start):
    """Words of the 'm <start> n' dump that follows in the log."""
    words = {}
    for line in lines:
        m = re.match(r"^([0-9A-F]{4}):\s+(.*?)\s+#", line)
        if not m:
            continue
        addr = int(m.group(1), 16)
        for i, w in enumerate(re.findall(r"([0-9A-F]{4})\*?", m.group(2))):
            words[addr + i] = int(w, 16)
    return words


def main(path):
    text = open(path, errors="replace").read()
    parts = re.split(r"^> m ", text, flags=re.M)
    if len(parts) < 3:
        print("no result dump in log (run did not reach the breakpoint)")
        return 1
    blk = dump(parts[1].splitlines(), 0x9B00)
    scr = dump(parts[2].splitlines(), 0x200)
    for row in range(12):
        chars, cols = "", ""
        for col in range(20):
            w = scr.get(0x200 + row * 20 + col, 0)
            card = (w >> 3) & 0xFF
            ch = chr(card + 32) if w & 0x0800 == 0 and 0 <= card <= 95 else "?"
            chars += ch
            cols += COLOUR[w & 7] if ch != " " else " "
        print(f"{chars}  {cols}")
    b = lambda a: blk.get(0x9B00 + a, 0) & 0xFF
    done, magic = b(1), b(0)
    run, skip, fail = b(2) | b(3) << 8, b(4) | b(5) << 8, b(6) | b(7) << 8
    line = b(8) | b(9) << 8
    print(f"magic={magic:02X} done={done} run={run} skip={skip} fail={fail} cline={line}")
    return 0 if magic == 0xA5 and done == 1 and fail == 0 else 1


if __name__ == "__main__":
    sys.exit(main(sys.argv[1]))

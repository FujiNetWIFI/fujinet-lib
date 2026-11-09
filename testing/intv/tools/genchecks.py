#!/usr/bin/env python3
"""Expand CHECK/SECTION lines in tests/*.t.bas into IntyBASIC.

  CHECK "name" @cline, condition   one check; name is <= 15 chars, cline is
                                   the line of the TEST() in the C source
  SECTION "text"                   section banner on the bottom row
  RANGE "name" @cline, expr, lo, hi   check lo <= expr <= hi
  CHECKI "Net %d ok" N @cline, idx, cond   N checks named "Net 1 ok".."Net N ok";
                                   idx (1..N) picks the name at run time
  CHECKL "A ok|B ok" @cline, idx, cond   the same for a list of names
  SKIPCHK "name" @cline            log a SKIP (the mailbox cannot do this check)
  STR name "text"                  ROM string str_<name>(0) + CONST SL_<name>;
                                   \\r \\n \\xHH escapes allowed

Writes build/<name>.bas for each source and build/names.bas, the packed
table of check names (two characters per word, 8 words per name).
"""
import re
import sys
from pathlib import Path

CHECK = re.compile(r'^(\s*)CHECK\s+"([^"]{1,15})"\s+@(\d+)\s*,\s*(.+?)\s*$')
CHECKL = re.compile(r'^(\s*)CHECKL\s+"([^"]+\|[^"]+)"\s+@(\d+)\s*,\s*([^,]+?)\s*,\s*(.+?)\s*$')
CHECKI = re.compile(r'^(\s*)CHECKI\s+"([^"]*%d[^"]*)"\s+(\d+)\s+@(\d+)\s*,\s*([^,]+?)\s*,\s*(.+?)\s*$')
SKIPCHK = re.compile(r'^(\s*)SKIPCHK\s+"([^"]{1,15})"\s+@(\d+)\s*$')
RANGE = re.compile(r'^(\s*)RANGE\s+"([^"]{1,15})"\s+@(\d+)\s*,\s*(.+)$')
STR = re.compile(r'^(\s*)STR\s+(\w+)\s+"(.*)"\s*$')
SECTION = re.compile(r'^(\s*)SECTION\s+"([^"]{1,20})"\s*$')
NAME_WORDS = 8


def main(out, sources):
    names = []
    out = Path(out)
    out.mkdir(exist_ok=True)
    for src in sources:
        lines = []
        for n, line in enumerate(Path(src).read_text().splitlines(), 1):
            m = CHECK.match(line)
            if m:
                ind, name, cline, cond = m.groups()
                if name not in names:
                    names.append(name)
                lines.append(f"{ind}#t_id = {names.index(name)} : #t_cl = {cline}"
                             f" : t_ok = 0 : IF {cond} THEN t_ok = 1")
                lines.append(f"{ind}GOSUB check : IF t_abort THEN RETURN")
                continue
            m = SKIPCHK.match(line)
            if m:
                ind, name, cline = m.groups()
                if name not in names:
                    names.append(name)
                lines.append(f"{ind}#t_id = {names.index(name)} : GOSUB skip_log")
                continue
            m = CHECKL.match(line)
            if m:
                ind, lst, cline, idx, cond = m.groups()
                first = len(names)
                for nm in lst.split("|"):
                    if len(nm) > 15:
                        sys.exit(f"{src}:{n}: name too long: {nm}")
                    names.append(nm)
                lines.append(f"{ind}#t_id = {first} + ({idx}) - 1 : #t_cl = {cline}"
                             f" : t_ok = 0 : IF {cond} THEN t_ok = 1")
                lines.append(f"{ind}GOSUB check : IF t_abort THEN RETURN")
                continue
            m = CHECKI.match(line)
            if m:
                ind, fmt, count, cline, idx, cond = m.groups()
                first = len(names)
                for k in range(1, int(count) + 1):
                    nm = fmt.replace("%d", str(k))
                    if len(nm) > 15:
                        sys.exit(f"{src}:{n}: name too long: {nm}")
                    names.append(nm)
                lines.append(f"{ind}#t_id = {first} + ({idx}) - 1 : #t_cl = {cline}"
                             f" : t_ok = 0 : IF {cond} THEN t_ok = 1")
                lines.append(f"{ind}GOSUB check : IF t_abort THEN RETURN")
                continue
            m = RANGE.match(line)
            if m:
                ind, name, cline, rest = m.groups()
                expr, lo, hi = [x.strip() for x in rest.rsplit(",", 2)]
                if name not in names:
                    names.append(name)
                lines.append(f"{ind}#rv = {expr} : #rlo = {lo} : #rhi = {hi} : GOSUB in_range")
                lines.append(f"{ind}#t_id = {names.index(name)} : #t_cl = {cline} : t_ok = t_in")
                lines.append(f"{ind}GOSUB check : IF t_abort THEN RETURN")
                continue
            m = STR.match(line)
            if m:
                ind, ident, text = m.groups()
                data = list(re.sub(r"\\(r|n|x[0-9A-Fa-f]{2})", lambda e: "\r" if e.group(1) == "r" else "\n" if e.group(1) == "n" else chr(int(e.group(1)[1:], 16)), text).encode("latin-1"))
                lines.append(f"    CONST SL_{ident} = {len(data)}")
                lines.append(f"str_{ident}:")
                # A NUL word follows the text, so SL_ + 1 compares include it.
                body = data + [0]
                for i in range(0, len(body), 12):
                    lines.append("    DATA " + ", ".join(str(b) for b in body[i:i + 12]))
                continue
            m = SECTION.match(line)
            if m:
                ind, text = m.groups()
                lines.append(f'{ind}PRINT AT 220 COLOR 6, "{text:<20}"')
                continue
            if re.match(r'^\s*(CHECK|CHECKI|CHECKL|SKIPCHK|SECTION|RANGE|STR)\b', line):
                sys.exit(f"{src}:{n}: bad CHECK/SECTION line")
            lines.append(line)
        name = Path(src).name.replace(".t.bas", ".bas")
        (out / name).write_text("\n".join(lines) + "\n")
    words = []
    for nm in names:
        nm = nm.ljust(NAME_WORDS * 2)
        words += [ord(nm[i]) | (ord(nm[i + 1]) << 8) for i in range(0, len(nm), 2)]
    body = ["' generated by tools/genchecks.py", "nm_tab:"]
    for i in range(0, len(words), 8):
        body.append("    DATA " + ", ".join(str(w) for w in words[i:i + 8]))
    (out / "names.bas").write_text("\n".join(body) + "\n")
    print(f"{len(names)} names", file=sys.stderr)


if __name__ == "__main__":
    main(sys.argv[1], sys.argv[2:])

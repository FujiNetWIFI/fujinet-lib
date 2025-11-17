#!/usr/bin/env python3
"""
generate_full_wiki.py

Usage:
  cd /home/rich/fujinet-lib
  python3 wiki/generate_full_wiki.py [--outdir wiki] [--headers fujinet-fuji.h,fujinet-network.h,fujinet-clock.h]

Creates:
  wiki/index.md
  wiki/generated/<function>.md
"""
import re
import os
import sys
import argparse
import subprocess
from pathlib import Path

parser = argparse.ArgumentParser()
parser.add_argument('--outdir', default='wiki', help='Output wiki directory (default: wiki)')
parser.add_argument('--headers', default='fujinet-fuji.h,fujinet-network.h,fujinet-clock.h',
                    help='Comma-separated header filenames relative to repo root')
args = parser.parse_args()

repo_root = Path.cwd()
outdir = (repo_root / args.outdir).resolve()
gen_dir = outdir / 'generated'
gen_dir.mkdir(parents=True, exist_ok=True)

headers = [repo_root / h.strip() for h in args.headers.split(',')]

def read_file(p):
    try:
        return p.read_text(encoding='utf-8')
    except Exception:
        return ''

# Heuristic: split header text on semicolons to get candidate declarations
def find_prototypes(text):
    candidates = []
    parts = re.split(r';', text)
    pos = 0
    for part in parts:
        seg = part.strip()
        if not seg:
            pos += len(part) + 1
            continue
        # ignore macros/typedefs/struct/enum lines
        if re.search(r'^\s*(#|typedef|struct|enum)\b', seg):
            pos += len(part) + 1
            continue
        # must contain '(' and ')'
        if '(' not in seg or ')' not in seg:
            pos += len(part) + 1
            continue
        # avoid function pointer typedefs that start with 'typedef'
        # attempt to match a function-like declaration ending with ')'
        candidates.append((seg, pos))
        pos += len(part) + 1
    return candidates

# Extract function name, return type, params from a segment
def parse_candidate(seg):
    seg = seg.strip()
    # remove possible leading attribute macros like 'extern ' or qualifiers
    # attempt regex: (ret_type) (name) (params)
    # allow multiline params; remove trailing comments inside
    s = re.sub(r'/\*.*?\*/', '', seg, flags=re.DOTALL)
    s = re.sub(r'//.*$', '', s, flags=re.MULTILINE)
    # Try to find the last identifier before '('
    m = re.search(r'([A-Za-z_][A-Za-z0-9_]*)\s*\(\s*([^)]*)\s*\)\s*$', s, flags=re.DOTALL)
    if not m:
        return None
    name = m.group(1)
    params = m.group(2).strip()
    pre = s[:m.start(1)]
    # derive return type = pre trimmed
    ret = pre.strip()
    # normalize whitespace
    ret = re.sub(r'\s+', ' ', ret)
    return {'name': name, 'params': params, 'ret': ret, 'raw': seg}

# find header comment block (/** ... */) immediately above a substring index
def find_comment_before(text, seg):
    # find segment position
    idx = text.find(seg)
    if idx == -1:
        return ''
    # search backward for /** ... */ that ends before idx
    # we'll find the last /** ... */ that ends before idx
    matches = list(re.finditer(r'/\*\*.*?\*/', text, flags=re.DOTALL))
    for m in reversed(matches):
        if m.end() <= idx:
            # return trimmed comment
            return m.group(0).strip()
    return ''

# run git grep to find implementations (best-effort). Returns list of matches.
def git_grep(fn_name):
    try:
        out = subprocess.check_output(['git', 'grep', '-n', '--break', '--heading', '-E', rf'{re.escape(fn_name)}\s*\('],
                                      cwd=repo_root, stderr=subprocess.DEVNULL, text=True)
        return [line.rstrip('\n') for line in out.splitlines()]
    except Exception:
        return []

# Search headers and build function map (header -> list of parsed prototypes)
function_map = {}  # header path -> list of entries
for h in headers:
    text = read_file(h)
    function_map[str(h)] = []
    if not text:
        continue
    candidates = find_prototypes(text)
    for seg, _pos in candidates:
        parsed = parse_candidate(seg)
        if parsed:
            # filter out some non-functions (e.g., macros that look like calls)
            # ensure name not keyword
            if parsed['name'] and not parsed['name'].startswith('('):
                parsed['comment'] = find_comment_before(text, seg)
                function_map[str(h)].append(parsed)

# Write index.md
index_path = outdir / 'index.md'
with index_path.open('w', encoding='utf-8') as f:
    f.write('# FujiNet-lib API Reference\n\n')
    f.write('This documentation is generated from the root header files:\n\n')
    for h in headers:
        f.write(f'- `{h.name}`\n')
    f.write('\n## Headers and Functions\n\n')
    for h in headers:
        f.write(f'### `{h.name}`\n\n')
        funcs = function_map.get(str(h), [])
        if not funcs:
            f.write('_No functions detected in this header._\n\n')
            continue
        f.write('Functions declared in this header:\n\n')
        for p in funcs:
            f.write(f'- [`{p["name"]}`](generated/{p["name"]}.md)\n')
        f.write('\n')

# Generate per-function pages
for h in headers:
    for p in function_map.get(str(h), []):
        name = p['name']
        out_file = gen_dir / f'{name}.md'
        with out_file.open('w', encoding='utf-8') as fh:
            fh.write(f'# {name}\n\n')
            fh.write(f'**Declared in:** `{h.name}`\n\n')
            fh.write('## Prototype\n\n')
            fh.write('```c\n')
            fh.write(p['raw'].strip() + ';\n')
            fh.write('```\n\n')
            fh.write('## Description\n\n')
            fh.write('_No description available — please add a detailed description of what this function does._\n\n')
            fh.write('## Parameters\n\n')
            if not p['params'] or p['params'].strip().lower() == 'void':
                fh.write('_This function takes no parameters._\n\n')
            else:
                # split parameters by commas at top-level (no parentheses support of nested)
                parts = [pp.strip() for pp in re.split(r',\s*(?![^()]*\))', p['params'])]
                fh.write('| Name | Type | Description |\n')
                fh.write('|---|---|---|\n')
                for part in parts:
                    # try to split last token as name
                    if not part:
                        continue
                    tokens = part.rsplit(None, 1)
                    if len(tokens) == 1:
                        ptype = tokens[0]
                        pname = ''
                    else:
                        ptype, pname = tokens
                    fh.write(f'| `{pname}` | `{ptype}` | _TODO: describe parameter_ |\n')
                fh.write('\n')
            fh.write('## Return Value\n\n')
            ret = p['ret'] or 'void'
            fh.write(f'- **Type:** `{ret}`\n\n')
            fh.write('- **Meaning:** _TODO: describe return value and error conditions._\n\n')
            fh.write('## Header notes\n\n')
            if p.get('comment'):
                fh.write('```\n')
                fh.write(p['comment'] + '\n')
                fh.write('```\n\n')
            else:
                fh.write('_No header comments found; placeholder for details._\n\n')
            fh.write('## Implementations (git grep results)\n\n')
            impls = git_grep(name)
            if impls:
                for line in impls:
                    fh.write(f'- {line}\n')
            else:
                fh.write('_No implementations found via git grep._\n')
            fh.write('\n')
            # Platform stub detection removed per user request.
            fh.write('\n----\n\n')
            fh.write('[Back to index](../index.md)\n')

print('Generation complete.')
print(f'Main index: {index_path}')
print(f'Per-function pages: {gen_dir}')
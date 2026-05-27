#!/usr/bin/env python3
from itertools import combinations, product

FIELD_FLAGS = {
  'A1': {
    'args': ['a1'],
    'aux': {0: 'a1'},
    'flags': ['FUJI_FIELD_AUX1'],
  },
  'A2': {
    'args': ['a2'],
    'aux': {1: 'a2'},
    'flags': ['FUJI_FIELD_AUX2'],
  },
  'A3': {
    'args': ['a3'],
    'aux': {2: 'a3'},
    'flags': ['FUJI_FIELD_AUX3'],
  },
  'A4': {
    'args': ['a4'],
    'aux': {3: 'a4'},
    'flags': ['FUJI_FIELD_AUX4'],
  },
  'B12': {
    'args': ['b12'],
    'aux': {
      0: '(uint8_t)((b12) >> 8)',
      1: '(uint8_t)(b12)',
    },
    'flags': ['FUJI_FIELD_AUX1', 'FUJI_FIELD_AUX2'],
  },
  'B34': {
    'args': ['b34'],
    'aux': {
      2: '(uint8_t)((b34) >> 8)',
      3: '(uint8_t)(b34)',
    },
    'flags': ['FUJI_FIELD_AUX3', 'FUJI_FIELD_AUX4'],
  },
  'C1234': {
    'args': ['c1234'],
    'aux': {
      0: '(uint8_t)((c1234) >> 24)',
      1: '(uint8_t)((c1234) >> 16)',
      2: '(uint8_t)((c1234) >> 8)',
      3: '(uint8_t)(c1234)',
    },
    'flags': [
      'FUJI_FIELD_AUX1', 'FUJI_FIELD_AUX2',
      'FUJI_FIELD_AUX3', 'FUJI_FIELD_AUX4'
    ],
  },
  'D': {
    'args': ['data', 'len'],
    'data': ('data', 'len'),
    'flags': ['FUJI_FIELD_DATA'],
  },
  'RV': {
    'args': ['reply', 'replylen'],
    'reply': ('reply', 'replylen'),
    'flags': [],
  }
}

def is_valid_combo(base_fields):
  # Exclusive: C1234 must not be combined with any other aux
  if 'C1234' in base_fields:
    others = {'A1', 'A2', 'A3', 'A4', 'B12', 'B34'}
    if any(f in base_fields for f in others):
      return False

  # B12 and B34 may coexist, but neither can be mixed with A1–A4
  if any(f in base_fields for f in ['B12', 'B34']):
    if any(f in base_fields for f in ['A1', 'A2', 'A3', 'A4']):
      return False

  # Hierarchy rules for A2–A4
  if 'A2' in base_fields and 'A1' not in base_fields:
    return False
  if 'A3' in base_fields and not {'A1', 'A2'}.issubset(base_fields):
    return False
  if 'A4' in base_fields and not {'A1', 'A2', 'A3'}.issubset(base_fields):
    return False

  return True

def gen_macro(fields):
  name = '_'.join(['FUJICALL'] + fields) if fields else 'FUJICALL'
  args = ['cmd']
  aux = ['0'] * 4
  data, dlen = 'NULL', '0'
  reply, rlen = 'NULL', '0'
  flags = []

  for f in fields:
    meta = FIELD_FLAGS[f]
    args.extend(meta.get('args', []))
    for i, val in meta.get('aux', {}).items():
      aux[i] = val
    flags.extend(meta.get('flags', []))
    if 'data' in meta:
      data, dlen = meta['data']
    if 'reply' in meta:
      reply, rlen = meta['reply']

  args_str = ', '.join(args)
  flag_expr = ' | '.join(sorted(set(flags))) or 'FUJI_FIELD_NONE'
  return (
    f"#define {name}({args_str}) \\\n"
    f"  fuji_bus_call((cmd), {flag_expr}, {aux[0]}, {aux[1]}, {aux[2]}, {aux[3]}, "
    f"{data}, {dlen}, {reply}, {rlen})"
  )

# === Generate ===

# Base sets that form the “core” of the call — no optional fields yet
base_options = [
  [], ['A1'], ['A1', 'A2'], ['A1', 'A2', 'A3'], ['A1', 'A2', 'A3', 'A4'],
  ['B12'], ['B34'], ['C1234']
]

# Filter base options by constraints
base_valid = [b for b in base_options if is_valid_combo(b)]

# Optional fields to add: D, RV, both, or none
optional_sets = [[], ['D'], ['RV'], ['D', 'RV']]

# Build and print all combinations
all_macros = []

for base in base_valid:
  for opt in optional_sets:
    full = base + opt
    all_macros.append(gen_macro(full))

# Sort by name for predictable order
for macro in sorted(all_macros):
  print(macro)
  print()

#!/usr/bin/env python3
from itertools import combinations

FUJICALL = "FUJICALL"

AUX_FLAGS = [
  ("A1", "FUJI_FIELD_AUX1", "a1"),
  ("A2", "FUJI_FIELD_AUX2", "a2"),
  ("A3", "FUJI_FIELD_AUX3", "a3"),
  ("A4", "FUJI_FIELD_AUX4", "a4"),
]

def gen_macro_name(prefix, parts):
  return f"{prefix}_{'_'.join(parts)}" if parts else prefix

def print_macro(name, flags, args, a_vals, data=False):
  flag_expr = ' | '.join(flags) if flags else '0'
  a1 = a_vals.get("a1", "0")
  a2 = a_vals.get("a2", "0")
  a3 = a_vals.get("a3", "0")
  a4 = a_vals.get("a4", "0")
  d_ptr = "data" if data else "NULL"
  d_len = "len" if data else "0"

  print(f"#define {name}({', '.join(args)}) \\")
  print(f"  fuji_bus_call(({args[0]}), {flag_expr}, {a1}, {a2}, {a3}, {a4}, {d_ptr}, {d_len})")
  print()

print("// Auto-generated Fuji call macros")
print("// Convention: A# = aux byte, B## = 16-bit, C#### = 32-bit, D = data")

# FUJICALL(cmd)
print_macro(FUJICALL, [], ["cmd"], {})

# Individual aux macros: FUJICALL_A1, FUJICALL_A1_A2, etc.
for i in range(1, 5):
  fields = AUX_FLAGS[:i]
  parts = [f[0] for f in fields]
  flags = [f[1] for f in fields]
  args = ["cmd"] + [f[2] for f in fields]
  a_vals = {f[2]: f[2] for f in fields}
  print_macro(gen_macro_name(FUJICALL, parts), flags, args, a_vals)
  print_macro(gen_macro_name(FUJICALL, parts + ["D"]), flags + ["FUJI_FIELD_DATA"],
              args + ["data", "len"], a_vals, data=True)

# B12: aux1 + aux2 as uint16_t
print_macro(FUJICALL + "_B12", ["FUJI_FIELD_AUX1", "FUJI_FIELD_AUX2"],
            ["cmd", "val16"],
            {"a1": "(uint8_t)((val16) & 0xFF)", "a2": "(uint8_t)(((val16) >> 8) & 0xFF)"})
print_macro(FUJICALL + "_B12_D", ["FUJI_FIELD_AUX1", "FUJI_FIELD_AUX2", "FUJI_FIELD_DATA"],
            ["cmd", "val16", "data", "len"],
            {"a1": "(uint8_t)((val16) & 0xFF)", "a2": "(uint8_t)(((val16) >> 8) & 0xFF)"},
            data=True)

# B34: aux3 + aux4
print_macro(FUJICALL + "_B34", ["FUJI_FIELD_AUX3", "FUJI_FIELD_AUX4"],
            ["cmd", "val16"],
            {"a3": "(uint8_t)((val16) & 0xFF)", "a4": "(uint8_t)(((val16) >> 8) & 0xFF)"})
print_macro(FUJICALL + "_B34_D", ["FUJI_FIELD_AUX3", "FUJI_FIELD_AUX4", "FUJI_FIELD_DATA"],
            ["cmd", "val16", "data", "len"],
            {"a3": "(uint8_t)((val16) & 0xFF)", "a4": "(uint8_t)(((val16) >> 8) & 0xFF)"},
            data=True)

# C1234: aux1–aux4 as 32-bit
print_macro(FUJICALL + "_C1234", [
    "FUJI_FIELD_AUX1", "FUJI_FIELD_AUX2",
    "FUJI_FIELD_AUX3", "FUJI_FIELD_AUX4"],
    ["cmd", "val32"],
    {
      "a1": "(uint8_t)((val32) & 0xFF)",
      "a2": "(uint8_t)(((val32) >> 8) & 0xFF)",
      "a3": "(uint8_t)(((val32) >> 16) & 0xFF)",
      "a4": "(uint8_t)(((val32) >> 24) & 0xFF)"
    })

print_macro(FUJICALL + "_C1234_D", [
    "FUJI_FIELD_AUX1", "FUJI_FIELD_AUX2",
    "FUJI_FIELD_AUX3", "FUJI_FIELD_AUX4",
    "FUJI_FIELD_DATA"],
    ["cmd", "val32", "data", "len"],
    {
      "a1": "(uint8_t)((val32) & 0xFF)",
      "a2": "(uint8_t)(((val32) >> 8) & 0xFF)",
      "a3": "(uint8_t)(((val32) >> 16) & 0xFF)",
      "a4": "(uint8_t)(((val32) >> 24) & 0xFF)"
    },
    data=True)

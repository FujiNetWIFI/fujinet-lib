# fujinet-lib tests for the Intellivision

The `testing/` programs, ported to IntyBASIC. The CP1610 has no C compiler,
so this talks to the FujiNet mailbox directly; it does not link fujinet-lib.
One cart holds all 14 programs behind a menu (keypad 1-9 and 0, ENTER for
the second page). Checks run in the same order as the C programs, a PASS is
green, and the first FAIL is red and stops the run. The failure screen names
the check and the line of the C `TEST()` it came from.

## Build

Needs IntyBASIC 1.5.2 or later (`OPTION MAP`) and `as1600`. Both are in
defoogi, so from the repo root:

    defoogi make -C testing/intv                  # menu cart
    defoogi make -C testing/intv PROG=appkey      # one program

`LIBDIR` holds `intybasic_prologue.asm` and `intybasic_epilogue.asm` and
defaults to defoogi's copy. Outside defoogi, point `INTYBASIC` and `LIBDIR` at
an IntyBASIC checkout:

    make INTYBASIC=/path/to/intybasic LIBDIR=/path/to/IntyBASIC/

The output is `build/intv/fnlib-test.bin` plus `.cfg`. The cart is banked
(`OPTION MAP 4`, one 8K bank per program), so it cannot be an Intellicart
`.rom`; run it with `jzintv build/intv/fnlib-test` (no extension) or load the
`.bin`/`.cfg` pair on a cart that pages ROM.

## Run

    tools/run.sh <program> [host:port]

runs the auto-run cart for one program headless in jzIntv against a FujiNet
(`--fujinet`, default `localhost:9975`) and prints the final screen. Use a
FujiNet you can lose the settings of: the tests rewrite host slots, device
slot 0, the host prefix, the system time zone, WiFi credentials and appkeys.
It needs an `SD` host slot, WiFi credentials in `fnconfig.ini`, and the same
internet hosts as the C suite.

## Layout

| file | |
|---|---|
| `main.bas` | menu, banks, one `run_<program>` per program |
| `harness.bas` | the C harness: log, PASS/FAIL, results screen, result block at `$9B00` |
| `fnlib.bas`, `fnnet.bas` | the mailbox and the fujinet-lib calls the tests use |
| `tests/t_<program>.t.bas` | one per C test file, same function order |
| `tools/genchecks.py` | expands `CHECK`/`RANGE`/`STR`... into IntyBASIC and builds the name table |

`CHECK "name" @line, cond` is one `TEST()`: the name is at most 15 columns and
`@line` is the line in the C source.

## Differences from the C suite

- The mailbox carries 256 bytes, so three things cannot be sent. They are
  logged as SKIP, not hidden: `fuji_put_device_slots` (304 bytes) in FUJIDEV,
  and NEW DISK's 262-byte request in DISK (5 checks). The oversized host
  slot write in FUJIDEV sends the 256-byte table without the 64 extra bytes.
- Line ending is CR LF, as on the other RS232-bus platforms.
- The 304 completion check keeps minutes, seconds and hundredths; it does not
  range-check the calendar date.
- A bank-only helper cannot be called from another bank; shared code lives in
  `fnlib.bas` and `fnnet.bas`.

## Known firmware failures

These are firmware behaviour; the C suite fails the same way.

- The held-peer HTTP checks (`html`) need a firmware whose first STATUS does
  not block until the peer closes.
- `set_device_filename` and `set_host_prefix` ignore an empty name, so the
  restore step fails on a slot or prefix that started empty.

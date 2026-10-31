# Contributing to fujinet-lib

fujinet-lib is the C library that retro computers use to talk to a FujiNet: one set of public
headers and one shared core, built for 12 targets (Adam, Apple II, Atari, C64, CoCo, H89 under
CP/M and HDOS, Lynx, TRS-80 Model II, MS-DOS, MSX, PMD 85) with four cross-compilers.

This is the working reference for anyone changing the code: new contributors, regulars, and AI
coding assistants alike.

## Before you start

- Check `git log --oneline -20 -- <paths>` and the open pull requests before touching an area. If
  recent work or an open PR overlaps, raise it before proceeding.
- For anything beyond a contained fix (a new public function, a change to a bus, a pattern you
  intend to repeat across platforms) describe the approach and get agreement first.
- If a maintainer asks you to hold off because another change is coming, stop.
- Ask on Discord or in the issue if any of this is unclear.

## Build

The cross-compilers are cc65 (Apple II, Atari, C64, Lynx), CMOC (CoCo), z88dk (Adam, H89, Model
II, MSX, PMD 85) and Open Watcom (MS-DOS). The `fozztexx/defoogi` container has all of them; CI
uses it.

```sh
make                      # every platform
make <platform>           # one, e.g. make coco; the library lands in r2r/<platform>/
make testing/<platform>   # the library, then that platform's test programs
```

The platform names are the `PLATFORMS` list in the top-level `Makefile`. `make clean` before a
build you mean to trust. Releases are in `README.md`.

## Layout

- `include/`: the public headers. `fujinet-platform.h` maps each compiler's predefined macro to
  one `FUJI_PLATFORM_*`.
- `common/`: code shared by every platform. It knows nothing about any platform.
- `bus/<platform>/`: each platform's bus code, and its own version of any function that has to
  differ there. `h89-cpm` and `h89-hdos` share `bus/h89/`.
- `testing/`: the test programs; see Tests.
- `makefiles/`: the shared build rules. Don't change them for one platform's needs.

## Design principles

Reviewers hold changes to this. A change that works but cuts against it will be sent back, so raise
the design first when yours doesn't fit.

One implementation, with platform variants, and never a platform branch inside shared code. C has
no subclasses, but the library has three ways to get the same effect:

- **The bus interface.** Shared code calls functions every platform implements in
  `bus/<platform>/`: `fuji_bus_call()`, `network_bus_read()`, `network_bus_write()`. These play the
  part of virtual methods. Wire protocol, timing and retries belong behind them, not in
  `common/`.
- **A platform's own version of a function.** When one platform needs a public function to behave
  differently, give it a file in `bus/<platform>/` named `<function>_<platform>.c` and route the
  public name to it in the header. The other platforms keep the shared version.

  ```c
  #if defined(FUJI_PLATFORM_ADAM)
  extern bool fuji_scan_for_networks_adam(uint8_t *count);
  #define fuji_scan_for_networks(count) fuji_scan_for_networks_adam(count)
  #else
  ...
  #endif
  ```

  `network_json_query_adam.c`, `network_seek_atari.c` and `fuji_read_appkey_c64.c` follow it.
- **A capability, not a platform list.** When shared code depends on what a bus can do, the
  platforms declare it once in `include/fujinet-bus.h` (`FUJI_VARIABLE_LEN_PACKETS`,
  `FUJI_BUS_CALL_VARARGS`) and shared code tests the capability. A new platform sets the flag;
  nothing in `common/` changes.

So:

- No `FUJI_PLATFORM_*` or compiler macro in a conditional anywhere in `common/`; don't add one, and
  remove the ones you touch. A header may test `FUJI_PLATFORM_*` to route a function or set a
  capability, as above.
- Test `FUJI_PLATFORM_*`, not the compiler's own macros (`__ATARI__`, `_CMOC_VERSION_`); only
  `fujinet-platform.h` reads those.
- Don't copy a shared function into a platform directory to change two lines. If the difference is
  in how bytes reach the device, it belongs behind the bus interface instead.
- Adding a platform means a `bus/<platform>/` directory, a line in `fujinet-platform.h` and the
  capability flags, not edits scattered through `common/`.
- `testing/check_no_build_ifdefs.py` checks `include/` and `common/` in CI. It covers only the
  macros it knows; reviewers hold the rest of the rule by hand.

## Size

These are 8-bit machines, and everything in `common/` links into every program that calls it,
CONFIG included. Apple II CONFIG has had only tens of bytes to spare.

- Keep arithmetic as narrow as the values allow. A 32-bit shift, multiply or divide pulls in a
  compiler runtime routine.
- Don't add a static buffer when the caller's buffer or an existing one will do.
- When a change grows shared code, link CONFIG against it (CI's "CONFIG check" does) and look at
  the map file if it is close.

## Validate a change before proposing it

CI builds every platform, runs the ifdef check, compiles the tests, and builds CONFIG for adam,
apple2, atari, c64 and coco against the PR. It runs no tests. So:

1. `make clean`, then `make` for every platform, not just the one you changed.
2. Build the test programs (`make testing/<platform>`) and run the ones that exercise your change on
   an emulator or hardware connected to a FujiNet (FujiNet-LWM works).
3. `git diff` and re-read every comment in it: each must still be true of the code as changed.
4. `git diff --stat` and confirm every file listed is one you meant to change.

## Tests

`testing/` builds one or more test disks per platform holding a menu and one program per subject
(`PRODUCTS` in `testing/Makefile`: FUJIDEV, DISK, NETFILE, NETWORK, and so on). They share the
harness in `testing/tests/harness.c`.

- Put a test in the program whose subject it is, not the one with a convenient fixture. DISK is for
  disk and device-slot tests; filesystem behavior goes in NETFILE.
- A fix that comes with a regression test is two PRs: the test first, which fails on current
  `main`, then the fix. The test PR says `Refs #N`; only the fix PR says `Fixes #N`.
- Run every test on every platform. `FN_BROKEN_<function>` in `testing/tests/broken-v5.h` skips a
  function that is broken on a platform and has an issue open; it is not for "doesn't apply here".
- Set up the state a test needs at the start of the test. The harness has no cleanup step after an
  abort, and a failed device may not take one anyway.
- A run stops at its first failed check, so describe a failure by that check.

## Code style

- Match the surrounding file. Don't reformat lines your change doesn't otherwise touch, and don't
  reorder includes.
- Give the values of one parameter (modes, types, command codes) an `enum`, not a list of
  `#define`s, and keep byte-wide fields `uint8_t`. A lone constant can stay a `#define`.
- Use fixed-width types for anything on the wire.

## Comments

Keep comments short: one or two lines above the code, or a brief trailing comment.

- Comment the *why* of a non-obvious choice (a bus timing, a firmware quirk, a size constraint). Don't
  restate the code.
- Don't narrate your edit: no "added to fix X", no dated notes, no before/after. That goes in the
  commit message and the PR.
- Don't leave commented-out code.

Before every commit, re-read each comment in `git diff`, including ones you didn't write whose code
you changed, and fix or delete any the change made untrue.

## Changelog

Add a line under `## [Unreleased]` in `Changelog.md` for a significant feature or for a fix to
something that didn't work in normal use, tagged by platform when it is one: `- [adam] ...`. Tests,
CI, build plumbing, refactors and docs get no entry.

## Scope of a change

One concern per pull request.

- Don't combine a refactor with a behavior change, a fix with a feature, or a move with either.
  Land the move first, then the change that needed it.
- Keep formatting and comment edits out of a functional diff.
- Every PR targets `main` and stands on its own; don't chain one onto another open branch.
- When you find an unrelated problem mid-change, leave it and open an issue for it.

## Commits and pull requests

- Work on a topic branch, never directly on `main`.
- The project squash-merges, and GitHub adds the `(#NNN)` suffix; don't type it yourself. Use
  imperative mood ("Add foo", not "Added foo").
- An optional `[tag]` prefix names the platform or area, and can stack: `[adam]`, `[coco][msx]`,
  `[testing]`, `[ci]`.
- `include/fujinet-version.h` changes only at release time; see `README.md`.
- Name no AI tool or its vendor in anything that lands in the repo or on GitHub: code, comments,
  docs, file and directory names, commit messages and PR descriptions. That includes the
  `Co-Authored-By:` trailer and "Generated with" footer some assistants add by default.
- Never commit an assistant's own instruction, memory or scratch files. `AGENTS.md` is the one such
  file the project keeps.

When the change fixes a tracked issue, put `Fixes #NNN` on its own line in the **pull request
description**, one keyword per issue. Use `Refs #NNN` for partial work.

## Write a short pull request description

Say why the change exists, what to look at first, and what you did not verify. Aim for one screen:
a sentence or two, a few bullets, then testing.

- One bullet per real change, not per file.
- Don't paste build output or test logs; quote the one line that matters, if any does.
- Don't narrate the work or restate the title.

This applies with particular force to descriptions written by an AI assistant, which by default run
several times longer than a reviewer needs. Write the short version.

## Say what you did not test

Say which platforms you built and which you ran tests on, emulator or hardware (and which machine),
and the FujiNet firmware version. "Builds everywhere, DISK passes in ADAMEm, not run on hardware"
is a good answer.

## Common mistakes

- **A platform `#if` in `common/`** instead of a `bus/<platform>/` file routed from the header, or
  a capability flag.
- **A copy of a shared function** in a platform directory, to change a few lines.
- **Testing a compiler macro** instead of `FUJI_PLATFORM_*`.
- **Growing shared code** without checking that CONFIG still fits.
- **Building one platform** after a change to `common/` or `include/`.
- **Assuming CI tests the library.** It only compiles it.
- **A fix and its test in one PR.**
- **A skipped test** where a form that runs everywhere would do.
- **A Changelog line** for a test, a build change or a refactor.
- **Leaving a comment that the change made untrue.**
- **A PR description that restates the diff.**

## Where to look

- `README.md` (building, releasing, the test images) and `README-bus.md` (which buses carry
  variable-length packets).
- `include/fujinet-bus.h` and `include/fujinet-bus-ezcall.h`: how a call reaches a platform's bus.
- `bus/<platform>/fujinet-bus-<platform>.c`: each platform's `fuji_bus_call()`.
- `git log --oneline -20 -- <path>` is the quickest way to learn a directory's local convention.

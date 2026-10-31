# AGENTS.md

The development standards for this repository are in [`CONTRIBUTING.md`](CONTRIBUTING.md).
**Read it before changing anything.** It is the single source of truth; this file only points at
it and repeats the few rules that cause the most damage when missed.

- Build with `make <platform>` or `make` for every platform; the cross-compilers are in the
  `fozztexx/defoogi` container. After touching `common/` or `include/`, build every platform.
- No platform conditional in `common/`. A platform that needs different behavior gets
  `bus/<platform>/<function>_<platform>.c`, routed from the public header, or a capability flag
  in `include/fujinet-bus.h`. Test `FUJI_PLATFORM_*`, never a compiler macro, outside
  `fujinet-platform.h`.
- Shared code links into every program, CONFIG included, on 8-bit machines with bytes to spare.
  Keep arithmetic narrow and check that CONFIG still links.
- CI only compiles. Run the test programs in `testing/` on an emulator or hardware with a FujiNet,
  and say in the PR what you ran and what you did not.
- A fix and its regression test are two PRs, the test first. Don't skip a test on a platform
  where a form that runs there would do.
- Changelog lines are for significant features and real fixes only.
- Keep comments short, keep one concern per pull request, and re-read the comments in your diff
  before committing to confirm the change did not make them untrue.
- Keep the pull request description short: why the change exists, what to look at, and what you
  did not test.

@CONTRIBUTING.md

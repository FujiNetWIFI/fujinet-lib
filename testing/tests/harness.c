/* =========================================================================
 * Minimal test harness
 * ========================================================================= */

#include "broken.h"

#include "harness.h"
#include "globals.h"
#include <fujinet-clock.h>

#ifdef _CMOC_VERSION_
#include <coco.h>
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif /* _CMOC_VERSION_ */

#ifdef BUILD_C64
#include "c64_find_load.h"
#endif /* BUILD_C64 */

#ifdef __CC65__
#include <conio.h>
#endif /* __CC65__ */

int tests_run     = 0;
int tests_passed  = 0;
int tests_skipped = 0;
int tests_failed  = 0;

void begin_testing()
{
  tests_run = tests_passed = tests_skipped = tests_failed = 0;
}

void end_testing(int code)
{
  printf("Press any key to exit\n");
#if defined(_CMOC_VERSION_)
  while (true)
  {
    if (inkey())
      break;
  }
#elif defined(__CC65__)
  cgetc();
#else /* everything else */
  getc(stdin);
#endif /* _CMOC_VERSION_ */

#ifdef BUILD_C64
  // The LOAD command is ridiculously long, reprint it if we can so
  // the user can arrow up and push return instead of retyping it.
  if (c64_load_command[0])
    printf("  %s\n", c64_load_command);
#endif /* BUILD_C64 */

  exit(code);
}

void print_versions()
{
  printf("fujinet-lib version %s\n", FNLIB_VERSION_FULL);
#ifdef GIT_VERSION
  printf("tests git commit ID %s\n", GIT_VERSION);
#endif

  strcpy(g.adapter.ace.fn_version, "BROKEN");
#ifndef FN_BROKEN_fuji_get_adapter_config_extended
  if (!fuji_get_adapter_config_extended(&g.adapter.ace))
    strcpy(g.adapter.ace.fn_version, "FAIL");
#endif
  printf("FujiNet: %-14s\n", g.adapter.ace.fn_version);
  printf("Platform: %s\n", PLATFORM);

  strcpy((char *) g.clock_fmt, "BROKEN");
#if !defined(FN_BROKEN_clock_get_time_UTC_ISO_STRING)
  if (clock_get_time(g.clock_fmt, UTC_ISO_STRING) != FN_ERR_OK)
    strcpy((char *) g.clock_fmt, "FAIL");
#elif !defined(FN_BROKEN_fuji_get_time)
  if (fuji_get_time(g.clock_fmt)) {
    int year, month, day, hour, minute, second;
    year = g.clock_fmt[0] * 100 + g.clock_fmt[1];
    month = g.clock_fmt[2];
    day = g.clock_fmt[3];
    hour = g.clock_fmt[4];
    minute = g.clock_fmt[5];
    second = g.clock_fmt[6];
    sprintf(g.clock_fmt, "%04d-%02d-%02dT%02d:%02d:%02d",
            year, month, day, hour, minute, second);
  }
  else
    strcpy((char *) g.clock_fmt, "FAIL");
#endif
  printf("  UTC ISO: %s\n", (char *)g.clock_fmt);

  return;
}

void test_abort(void)
{
  printf("\n*** ABORTED after first failure ***\n");
  printf("=== Results: %d passed, %d skipped, %d failed, %d total ===\n",
         tests_passed, tests_skipped, tests_failed, tests_run);
  print_versions();
  end_testing(1);
}

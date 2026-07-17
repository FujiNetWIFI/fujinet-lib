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

int tests_run     = 0;
int tests_passed  = 0;
int tests_skipped = 0;
int tests_failed  = 0;

void end_testing(int code)
{
  printf("Press any key to exit\n");
#ifdef _CMOC_VERSION_
  while (true)
  {
    if (inkey())
      break;
  }
#else /* ! _CMOC_VERSION_ */
  getc(stdin);
#endif /* _CMOC_VERSION_ */
  exit(code);
}

void print_versions()
{
  bool err;


  printf("fujinet-lib version %s\n", FNLIB_VERSION_FULL);
#ifdef GIT_VERSION
  printf("tests git commit ID %s\n", GIT_VERSION);
#endif

  err = true;
  strcpy(g.adapter.ace.fn_version, "BROKEN");
#ifndef FN_BROKEN_fuji_get_adapter_config_extended
  err = fuji_get_adapter_config_extended(&g.adapter.ace) == 0;
  if (err)
    strcpy(g.adapter.ace.fn_version, "FAIL");
#endif
  printf("FujiNet: %-14s\n", g.adapter.ace.fn_version);

  err = true;
  strcpy((char *) g.clock_fmt, "BROKEN");
#ifndef FN_BROKEN_clock_get_time_UTC_ISO_STRING
  err = clock_get_time(g.clock_fmt, UTC_ISO_STRING) != FN_ERR_OK;
  if (err)
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

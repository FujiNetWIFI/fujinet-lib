/* =========================================================================
 * Minimal test harness
 * ========================================================================= */

#include "harness.h"

#ifdef _CMOC_VERSION_
#include <coco.h>
#else
#include <stdio.h>
#include <stdlib.h>
#endif /* _CMOC_VERSION_ */

int tests_run     = 0;
int tests_passed  = 0;
int tests_skipped = 0;
int tests_failed  = 0;

void end_testing(int code)
{
#ifdef BUILD_ATARI
  (void) code;
  while (1)
    ;
#else
  exit(code);
#endif
}

void test_abort(void)
{
  printf("\n*** ABORTED after first failure ***\n");
  printf("=== Results: %d passed, %d skipped, %d failed, %d total ===\n",
         tests_passed, tests_skipped, tests_failed, tests_run);
  printf("fujinet-lib version %s\n", FNLIB_VERSION_FULL);
  end_testing(1);
}

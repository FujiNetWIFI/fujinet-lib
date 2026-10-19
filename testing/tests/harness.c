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

#if defined(__CC65__) || defined(__WATCOMC__) || defined(FUJI_PLATFORM_ADAM)
#include <conio.h>
#endif /* __CC65__ || __WATCOMC__ || FUJI_PLATFORM_ADAM */
#ifdef __C64__
#include <c64.h>
#endif /* __C64__ */
#ifdef __WATCOMC__
#include <i86.h>
#endif /* __WATCOMC__ */

#ifdef BUILD_APPLE2
extern const char *computer_model();
#endif /* BUILD_APPLE2 */

int tests_run     = 0;
int tests_passed  = 0;
int tests_skipped = 0;
int tests_failed  = 0;

void begin_testing()
{
  tests_run = tests_passed = tests_skipped = tests_failed = 0;
}

#ifndef BUILD_LYNX
/* Only SPACE or ENTER ends the run, so a screenshot shortcut or a stray
 * modifier cannot (#156). The Lynx has no keyboard and keeps any key. */
static bool exit_key(int k)
{
  return k == ' ' || k == '\r' || k == '\n'
#ifdef CH_ENTER
    || k == CH_ENTER
#endif
    ;
}
#endif /* BUILD_LYNX */

void end_testing(int code)
{
#ifdef BUILD_LYNX
  printf("Press any key to exit\n");
  cgetc();
#else
  int k;

  printf("Press SPACE or ENTER to exit\n");
  do {
#if defined(_CMOC_VERSION_)
    k = inkey();
#elif defined(__CC65__)
    k = cgetc();
#elif defined(__WATCOMC__)
    k = getch();
#elif defined(__Z88DK)
    k = fgetc_cons();
#else /* everything else */
    k = getc(stdin);
#endif /* _CMOC_VERSION_ */
  } while (!exit_key(k));
#endif /* BUILD_LYNX */

#if defined(_CMOC_VERSION_)
  /* Back to the ROM's black, which the menu expects in these slots */
  if (isCoCo3) {
    palette(10, 0);
    palette(12, 0);
  }
#endif /* _CMOC_VERSION_ */

#ifdef BUILD_C64
  // The LOAD command is ridiculously long, reprint it if we can so
  // the user can arrow up and push return instead of retyping it.
  if (c64_load_command[0])
    printf("  %s\n", c64_load_command);
#endif /* BUILD_C64 */

  exit(code);
}

#define HL_GREEN  1
#define HL_YELLOW 2
#define HL_RED    3

/* Pass, clean ID and the all-passed line green; dirty ID yellow; fail and
 * the abort line red (#96). Atari, Apple II, the H89 terminal and MS-DOS in
 * the monochrome text mode show yellow and red as inverse video. */
void tprint(uint8_t what, const char *s)
{
  uint8_t hl;

  switch (what) {
  case TP_PASS: case TP_ALL_PASSED: case TP_ID_CLEAN: hl = HL_GREEN; break;
  case TP_ID_DIRTY: hl = HL_YELLOW; break;
  case TP_FAIL: case TP_ABORTED: hl = HL_RED; break;
  default: hl = 0; break;
  }
  if (!hl) {
    printf("%s", s);
    return;
  }
#if defined(_CMOC_VERSION_)
  if (isCoCo3) {
    /* Foreground slots 9, 10 and 12, set in init_video() */
    attr(hl == HL_GREEN ? 1 : hl == HL_YELLOW ? 2 : 4, 0, 0, 0);
    printf("%s", s);
    attr(0, 0, 0, 0);
  } else
    printf("%s", s);
#elif defined(__C64__)
  {
    unsigned char old = textcolor(hl == HL_GREEN ? COLOR_GREEN
                                  : hl == HL_YELLOW ? COLOR_YELLOW : COLOR_RED);

    printf("%s", s);
    textcolor(old);
  }
#elif defined(FUJI_PLATFORM_ADAM)
  /* conio's PC color numbers: 2 green, 14 yellow, 4 red, 15 white */
  textcolor(hl == HL_GREEN ? 2 : hl == HL_YELLOW ? 14 : 4);
  printf("%s", s);
  textcolor(15);
#elif defined(__WATCOMC__)
  {
    union REGS r;
    uint8_t row, col, attr;

    fflush(stdout);
    r.h.ah = 0x0F;
    int86(0x10, &r, &r);
    if (r.h.al == 7)
      attr = hl == HL_GREEN ? 0x07 : 0x70;
    else
      attr = hl == HL_GREEN ? 0x0A : hl == HL_YELLOW ? 0x0E : 0x0C;
    r.h.ah = 3; r.h.bh = 0;
    int86(0x10, &r, &r);
    row = r.h.dh; col = r.h.dl;
    for (; *s; s++, col++) {
      r.h.ah = 2; r.h.bh = 0; r.h.dh = row; r.h.dl = col;
      int86(0x10, &r, &r);
      r.h.ah = 9; r.h.al = *s; r.h.bh = 0; r.h.bl = attr; r.x.cx = 1;
      int86(0x10, &r, &r);
    }
    r.h.ah = 2; r.h.bh = 0; r.h.dh = row; r.h.dl = col;
    int86(0x10, &r, &r);
  }
#elif defined(__ATARI__)
  for (; *s; s++)
    putchar(hl == HL_GREEN ? *s : (*s | 0x80));
#elif defined(__APPLE2__)
  if (hl != HL_GREEN)
    *(uint8_t *) 0x32 = 0x3F;
  printf("%s", s);
  *(uint8_t *) 0x32 = 0xFF;
#elif defined(FUJI_PLATFORM_H89)
  if (hl != HL_GREEN)
    printf("\x1bp%s\x1bq", s);
  else
    printf("%s", s);
#else
  printf("%s", s);
#endif
}

/* The commit IDs: green when clean, yellow when the build was dirty. */
static void print_id(const char *label, const char *id)
{
  printf("%s", label);
  tprint(strchr(id, '*') ? TP_ID_DIRTY : TP_ID_CLEAN, id);
  printf("\n");
}

void print_versions()
{
  printf("fujinet-lib version %s\n", FNLIB_VERSION_FULL);
#ifdef GIT_VERSION
  print_id("tests git commit ID ", GIT_VERSION);
#else
#  error "Need GIT_VERSION"
#endif

  strcpy(g.adapter.ace.fn_version, "BROKEN");
#ifndef FN_BROKEN_fuji_get_adapter_config_extended
  if (!fuji_get_adapter_config_extended(&g.adapter.ace))
    strcpy(g.adapter.ace.fn_version, "FAIL");
#endif
  printf("Test program: %s\n", test_program);
  print_id("FujiNet: ", g.adapter.ace.fn_version);
#ifdef BUILD_APPLE2
  printf("Platform: %s\n", computer_model());
#else
  printf("Platform: %s\n", PLATFORM);
#endif /* BUILD_APPLE2 */

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

void test_check(uint8_t ok, const char *name, int line)
{
  tests_run++;
  if (ok) {
    tprint(TP_PASS, "PASS");
    printf("  %s\n", name);
    tests_passed++;
  } else {
    tprint(TP_FAIL, "FAIL");
    printf("  %s  (line %d)\n", name, line);
    tests_failed++;
    test_abort();
  }
}

void test_abort(void)
{
  printf("\n");
  tprint(TP_ABORTED, "*** ABORTED after first failure ***");
  printf("\n");
  printf("=== Results: %d passed, %d skipped, %d failed, %d total ===\n",
         tests_passed, tests_skipped, tests_failed, tests_run);
  print_versions();
  end_testing(1);
}

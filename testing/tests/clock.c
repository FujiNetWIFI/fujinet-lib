#include "broken.h"

#include "harness.h"
#include "globals.h"
#include "print_hex.h"
#include <fujinet-clock.h>

#ifndef _CMOC_VERSION_
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#endif /* _CMOC_VERSION_ */

void test_clock_tz(void)
{
  uint8_t err;

  SECTION("clock timezone get/set");

  memset(g.clock_tz_set.original, 0, sizeof(g.clock_tz_set.original));
#ifdef FN_BROKEN_clock_get_tz
  SKIP(clock_get_tz);
#else
  err = clock_get_tz(g.clock_tz_set.original);
  TEST("clock_get_tz succeeds", err == FN_ERR_OK);
  TEST("clock_get_tz returns non-empty string", g.clock_tz_set.original[0] != '\0');
  printf("  Current TZ: %s\n", g.clock_tz_set.original);
#endif

#ifdef FN_BROKEN_clock_set_tz
  SKIP(clock_set_tz);
#else
  err = clock_set_tz("America/New_York");
  TEST("clock_set_tz(America/New_York) succeeds", err == FN_ERR_OK);

  memset(g.clock_tz_set.readback, 0, sizeof(g.clock_tz_set.readback));
  err = clock_get_tz(g.clock_tz_set.readback);
  TEST("clock_get_tz after set succeeds", err == FN_ERR_OK);
  TEST("TZ read-back matches set value",
       strcmp(g.clock_tz_set.readback, "America/New_York") == 0);

  clock_set_tz(g.clock_tz_set.original);
  printf("  TZ restored to: %s\n", g.clock_tz_set.original);
#endif

  END_OF_TEST();
}

void test_clock_simple_binary(void)
{
  uint8_t err;

  SECTION("clock_get_time SIMPLE_BINARY");

#ifdef FN_BROKEN_clock_get_time_SIMPLE_BINARY
  SKIP(clock_get_time);
#else
  memset(g.clock_fmt, 0, sizeof(g.clock_fmt));
  err = clock_get_time(g.clock_fmt, SIMPLE_BINARY);
  TEST("clock_get_time(SIMPLE_BINARY) returns FN_ERR_OK", err == FN_ERR_OK);
  TEST("SIMPLE_BINARY century is 20", g.clock_fmt[0] == 20);
  TEST("SIMPLE_BINARY year is in 0-99", g.clock_fmt[1] <= 99);
  TEST("SIMPLE_BINARY month is 1-12", g.clock_fmt[2] >= 1 && g.clock_fmt[2] <= 12);
  TEST("SIMPLE_BINARY day is 1-31", g.clock_fmt[3] >= 1 && g.clock_fmt[3] <= 31);
  TEST("SIMPLE_BINARY hour is 0-23", g.clock_fmt[4] <= 23);
  TEST("SIMPLE_BINARY min is 0-59", g.clock_fmt[5] <= 59);
  TEST("SIMPLE_BINARY sec is 0-59", g.clock_fmt[6] <= 59);
  printf("  DateTime: 20%02u-%02u-%02u %02u:%02u:%02u\n",
         g.clock_fmt[1], g.clock_fmt[2], g.clock_fmt[3],
         g.clock_fmt[4], g.clock_fmt[5], g.clock_fmt[6]);
#endif

  END_OF_TEST();
}

void test_clock_prodos_binary(void)
{
  uint8_t err;
  bool nonzero;

  SECTION("clock_get_time PRODOS_BINARY");

#ifdef FN_BROKEN_clock_get_time_PRODOS_BINARY
  SKIP(clock_get_time);
#else
  memset(g.clock_fmt, 0, sizeof(g.clock_fmt));
  err = clock_get_time(g.clock_fmt, PRODOS_BINARY);
  TEST("clock_get_time(PRODOS_BINARY) returns FN_ERR_OK", err == FN_ERR_OK);
  print_hex("PRODOS_BINARY", g.clock_fmt, 4);
  nonzero = (g.clock_fmt[0] | g.clock_fmt[1] |
             g.clock_fmt[2] | g.clock_fmt[3]) != 0;
  TEST("PRODOS_BINARY result is non-zero", nonzero);
#endif

  END_OF_TEST();
}

void test_clock_apetime_binary(void)
{
  uint8_t err;

  SECTION("clock_get_time APETIME_BINARY");

#ifdef FN_BROKEN_clock_get_time_APETIME_BINARY
  SKIP(clock_get_time);
#else
  memset(g.clock_fmt, 0, sizeof(g.clock_fmt));
  err = clock_get_time(g.clock_fmt, APETIME_BINARY);
  TEST("clock_get_time(APETIME_BINARY) returns FN_ERR_OK", err == FN_ERR_OK);
  TEST("APETIME day is 1-31", g.clock_fmt[0] >= 1 && g.clock_fmt[0] <= 31);
  TEST("APETIME month is 1-12", g.clock_fmt[1] >= 1 && g.clock_fmt[1] <= 12);
  TEST("APETIME year is 0-99", g.clock_fmt[2] <= 99);
  TEST("APETIME hour is 0-23", g.clock_fmt[3] <= 23);
  TEST("APETIME min is 0-59", g.clock_fmt[4] <= 59);
  TEST("APETIME sec is 0-59", g.clock_fmt[5] <= 59);
  printf("  APETime: %02u/%02u/%02u %02u:%02u:%02u\n",
         g.clock_fmt[0], g.clock_fmt[1], g.clock_fmt[2],
         g.clock_fmt[3], g.clock_fmt[4], g.clock_fmt[5]);
#endif

  END_OF_TEST();
}

void test_clock_tz_iso_string(void)
{
  uint8_t err;
  uint8_t len;

  SECTION("clock_get_time TZ_ISO_STRING");

#ifdef FN_BROKEN_clock_get_time_TZ_ISO_STRING
  SKIP(clock_get_time);
#else
  memset(g.clock_fmt, 0, sizeof(g.clock_fmt));
  err = clock_get_time(g.clock_fmt, TZ_ISO_STRING);
  TEST("clock_get_time(TZ_ISO_STRING) returns FN_ERR_OK", err == FN_ERR_OK);
  TEST("ISO string starts with '20'", g.clock_fmt[0] == '2' && g.clock_fmt[1] == '0');
  TEST("ISO string has 'T' separator at pos 10", g.clock_fmt[10] == 'T');
  TEST("ISO string has ':' at pos 13", g.clock_fmt[13] == ':');
  TEST("ISO string has ':' at pos 16", g.clock_fmt[16] == ':');
  len = (uint8_t)strlen((char *)g.clock_fmt);
  TEST("ISO string length is 24", len == 24);
  printf("  TZ ISO: %s\n", (char *)g.clock_fmt);
#endif

  END_OF_TEST();
}

void test_clock_utc_iso_string(void)
{
  uint8_t err;
  uint8_t len;

  SECTION("clock_get_time UTC_ISO_STRING");

#ifdef FN_BROKEN_clock_get_time_UTC_ISO_STRING
  SKIP(clock_get_time);
#else
  memset(g.clock_fmt, 0, sizeof(g.clock_fmt));
  err = clock_get_time(g.clock_fmt, UTC_ISO_STRING);
  TEST("clock_get_time(UTC_ISO_STRING) returns FN_ERR_OK", err == FN_ERR_OK);
  TEST("UTC ISO string starts with '20'", g.clock_fmt[0] == '2' && g.clock_fmt[1] == '0');
  len = (uint8_t)strlen((char *)g.clock_fmt);
  TEST("UTC ISO string ends with '+0000'", len >= 5 && strcmp((char *)g.clock_fmt + len - 5, "+0000") == 0);
  printf("  UTC ISO: %s\n", (char *)g.clock_fmt);
#endif

  END_OF_TEST();
}

void test_clock_apple3_sos(void)
{
  uint8_t err;

  SECTION("clock_get_time APPLE3_SOS_BINARY");

#ifdef FN_BROKEN_clock_get_time_APPLE3_SOS_BINARY
  SKIP(clock_get_time);
#else
  memset(g.clock_fmt, 0, sizeof(g.clock_fmt));
  err = clock_get_time(g.clock_fmt, APPLE3_SOS_BINARY);
  TEST("clock_get_time(APPLE3_SOS_BINARY) returns FN_ERR_OK", err == FN_ERR_OK);
  print_hex("APPLE3_SOS", g.clock_fmt, 18);
  TEST("APPLE3_SOS reserved byte[8] is 0", g.clock_fmt[8] == '0');
#endif

  END_OF_TEST();
}

void test_clock_get_time_tz(void)
{
  uint8_t err;

  SECTION("clock_get_time_tz (per-call TZ, does not affect system TZ)");

#ifdef FN_BROKEN_clock_get_time_tz
  SKIP(clock_get_time_tz);
#else
  memset(g.clock_tz_call.utc, 0, sizeof(g.clock_tz_call.utc));
  memset(g.clock_tz_call.ny,  0, sizeof(g.clock_tz_call.ny));
  memset(g.clock_tz_call.tok, 0, sizeof(g.clock_tz_call.tok));

  err = clock_get_time_tz(g.clock_tz_call.utc, "UTC", TZ_ISO_STRING);
  TEST("clock_get_time_tz(UTC, ISO) succeeds", err == FN_ERR_OK);
  printf("  UTC time:   %s\n", (char *)g.clock_tz_call.utc);

  err = clock_get_time_tz(g.clock_tz_call.ny, "America/New_York", TZ_ISO_STRING);
  TEST("clock_get_time_tz(New_York, ISO) succeeds", err == FN_ERR_OK);
  printf("  NY time:    %s\n", (char *)g.clock_tz_call.ny);

  err = clock_get_time_tz(g.clock_tz_call.tok, "Asia/Tokyo", TZ_ISO_STRING);
  TEST("clock_get_time_tz(Tokyo, ISO) succeeds", err == FN_ERR_OK);
  printf("  Tokyo time: %s\n", (char *)g.clock_tz_call.tok);

  TEST("UTC and New_York times differ", strcmp((char *)g.clock_tz_call.utc, (char *)g.clock_tz_call.ny) != 0);
  TEST("UTC and Tokyo times differ", strcmp((char *)g.clock_tz_call.utc, (char *)g.clock_tz_call.tok) != 0);

  memset(g.clock_tz_call.sys_tz, 0, sizeof(g.clock_tz_call.sys_tz));
  clock_get_tz(g.clock_tz_call.sys_tz);
  TEST("clock_get_time_tz did not change system TZ to Asia/Tokyo", strcmp(g.clock_tz_call.sys_tz, "Asia/Tokyo") != 0);
#endif

  END_OF_TEST();
}

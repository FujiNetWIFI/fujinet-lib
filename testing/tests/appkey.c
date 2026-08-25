#include "broken.h"

#include "harness.h"
#include "globals.h"
#include "cmp_hex.h"
#include <fujinet-fuji.h>

#ifndef _CMOC_VERSION_
#include <stdio.h>
#include <string.h>
#endif /* _CMOC_VERSION_ */

/* Unused ids under the creator/app this suite owns: 54450110/54450111.key */
#define APPKEY_KEY_EMPTY   0x10
#define APPKEY_KEY_MISSING 0x11

/* Non-zero fill so "came back zeroed" cannot pass on a buffer we zeroed. */
#define APPKEY_SENTINEL    0xEE

static bool appkey_all_zero(const uint8_t *buf, uint16_t len)
{
  uint16_t i;

  for (i = 0; i < len; i++)
    if (buf[i] != 0)
      return false;
  return true;
}

void test_fuji_appkey(void)
{
  bool ok;
  uint16_t count;

  SECTION("fuji appkey read/write");

#ifdef FN_BROKEN_fuji_set_appkey_details
  SKIP(fuji_set_appkey_details);
#else
  fuji_set_appkey_details(0x5445, 0x01, DEFAULT);
#endif

#ifdef FN_BROKEN_fuji_write_appkey
  SKIP(fuji_write_appkey);
#else
#ifdef FN_BROKEN_fuji_read_appkey
  SKIP(fuji_read_appkey);
#else

  /* --- First write: fill with 0xAB, distinctive bytes at start and end --- */
  memset(g.appkey.write, 0xAB, sizeof(g.appkey.write));
  g.appkey.write[0]  = 0x01;
  g.appkey.write[63] = 0x02;

  ok = fuji_write_appkey(0, sizeof(g.appkey.write), g.appkey.write);
  TEST("fuji_write_appkey (first) succeeds", ok);

  memset(g.appkey.read, 0, sizeof(g.appkey.read));
  count = 0;
  ok = fuji_read_appkey(0, &count, g.appkey.read);
  TEST("fuji_read_appkey (first) succeeds", ok);
  TEST("fuji_read_appkey (first) returned 64 bytes", count == 64);
  if ( memcmp(g.appkey.read, g.appkey.write, 64) != 0)
    cmp_hex("read", g.appkey.read, 64,
            "writ", g.appkey.write, 64);
  TEST("fuji_read_appkey (first) data matches write",
       memcmp(g.appkey.read, g.appkey.write, 64) == 0);

  /* --- Second write: invert the pattern to prove read reflects new data --- */
  memset(g.appkey.write, 0x54, sizeof(g.appkey.write));
  g.appkey.write[0]  = 0x03;
  g.appkey.write[63] = 0x04;

  ok = fuji_write_appkey(0, sizeof(g.appkey.write), g.appkey.write);
  TEST("fuji_write_appkey (second) succeeds", ok);

  memset(g.appkey.read, 0, sizeof(g.appkey.read));
  count = 0;
  ok = fuji_read_appkey(0, &count, g.appkey.read);
  TEST("fuji_read_appkey (second) succeeds", ok);
  TEST("fuji_read_appkey (second) returned 64 bytes", count == 64);
  TEST("fuji_read_appkey (second) data matches new write",
       memcmp(g.appkey.read, g.appkey.write, 64) == 0);

  /* --- Empty appkey: zero length write --- */
  ok = fuji_write_appkey(APPKEY_KEY_EMPTY, 0, g.appkey.write);
  TEST("fuji_write_appkey (empty) succeeds", ok);

  memset(g.appkey.read, APPKEY_SENTINEL, sizeof(g.appkey.read));
  count = APPKEY_SENTINEL;
  ok = fuji_read_appkey(APPKEY_KEY_EMPTY, &count, g.appkey.read);
  TEST("fuji_read_appkey (empty) succeeds", ok);
  printf("  Empty appkey read returned %u bytes\n", (unsigned) count);
  TEST("fuji_read_appkey (empty) length within bounds",
       count <= MAX_APPKEY_LEN);
  TEST("fuji_read_appkey (empty) returns no garbage",
       count <= MAX_APPKEY_LEN && appkey_all_zero(g.appkey.read, count));

  /* --- Write it back, the load-modify-save path an app takes --- */
  ok = fuji_write_appkey(APPKEY_KEY_EMPTY, count, g.appkey.read);
  TEST("fuji_write_appkey (empty, written back) succeeds", ok);

  memset(g.appkey.read, APPKEY_SENTINEL, sizeof(g.appkey.read));
  count = APPKEY_SENTINEL;
  ok = fuji_read_appkey(APPKEY_KEY_EMPTY, &count, g.appkey.read);
  TEST("fuji_read_appkey (empty, after write back) succeeds", ok);
  printf("  Written back empty appkey read returned %u bytes\n", (unsigned) count);
  TEST("fuji_read_appkey (empty, after write back) length within bounds",
       count <= MAX_APPKEY_LEN);
  TEST("fuji_read_appkey (empty, after write back) returns no garbage",
       count <= MAX_APPKEY_LEN && appkey_all_zero(g.appkey.read, count));

  /* --- A key that was never created --- */
  memset(g.appkey.read, APPKEY_SENTINEL, sizeof(g.appkey.read));
  count = APPKEY_SENTINEL;
  ok = fuji_read_appkey(APPKEY_KEY_MISSING, &count, g.appkey.read);
  TEST("fuji_read_appkey (never written) succeeds", ok);
  printf("  Never written appkey read returned %u bytes\n", (unsigned) count);
  TEST("fuji_read_appkey (never written) length within bounds",
       count <= MAX_APPKEY_LEN);
  TEST("fuji_read_appkey (never written) returns no garbage",
       count <= MAX_APPKEY_LEN && appkey_all_zero(g.appkey.read, count));
#endif
#endif

  END_OF_TEST();
}

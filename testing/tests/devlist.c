#include "broken.h"

#include "devlist.h"
#include "harness.h"
#include "globals.h"
#include "cmp_hex.h"

#include <fujinet-clock.h>
#include <fujinet-network.h>

#ifndef _CMOC_VERSION_
#include <stdio.h>
#include <string.h>
#endif /* _CMOC_VERSION_ */

#if defined(BUILD_ADAM) || defined(BUILD_ADAM_CPM)
#define MAX_NETWORK_DEVICES 2
#else
#ifndef FUJI_DEVICEID_NETWORK
#define FUJI_DEVICEID_NETWORK             0x71
#endif
#ifndef FUJI_DEVICEID_NETWORK_LAST
#define FUJI_DEVICEID_NETWORK_LAST        0x78
#endif
#define MAX_NETWORK_DEVICES (FUJI_DEVICEID_NETWORK_LAST - FUJI_DEVICEID_NETWORK + 1)
#endif /* BUILD_ADAM || BUILD_ADAM_CPM */

void test_fujinet_exists(void)
{
  bool ok;

  SECTION("FujiNet device");

#ifdef FN_BROKEN_fuji_get_adapter_config
  SKIP(fujinet_exists);
#endif
  ok = fuji_get_adapter_config(&g.adapter.ac);
  TEST("FujiNet online", ok);
  END_OF_TEST();
}

void test_clock_exists(void)
{
  uint8_t err;

  SECTION("Clock device");

#ifdef FN_BROKEN_clock_get_time_APETIME_BINARY
  SKIP(clock_exists);
#else
  err = clock_get_time(g.clock_fmt, APETIME_BINARY);
  TEST("Clock online", err == FN_ERR_OK);
#endif
  END_OF_TEST();
}

void test_network_exists(void)
{
  uint8_t idx, err, unit;
  char *r;
  char buffer[40], spec[40];

  SECTION("N: device");

#if defined(FN_BROKEN_network_fs_cd) || defined(FN_BROKEN_network_fs_pwd)
  SKIP(network_exists);
#else
  // First set every network device to a unique prefix
  for (idx = 0; idx < MAX_NETWORK_DEVICES; idx++) {
    unit = idx + '1';
    sprintf(buffer, "N%c:/%c-check-%c", unit, unit, unit);
    err = network_fs_cd(buffer);
    if (err != FN_ERR_OK)
      break;
  }
  TEST("All network devices respond", err == FN_ERR_OK);

  // Now check that each device still has a unique prefix
  for (idx = 0; idx < MAX_NETWORK_DEVICES; idx++) {
    unit = idx + '1';
    sprintf(spec, "N%c:", unit);
    sprintf(buffer, "/%c-check-%c", unit, unit);
    err = network_fs_pwd(spec, g.fs.path);
    if (err != FN_ERR_OK)
      break;
    r = strrchr(g.fs.path, '/');
    if (r && r > &g.fs.path[1])
      *r = 0;
    if (strcmp(g.fs.path, buffer) != 0)
      cmp_hex("orig", (uint8_t *) buffer, strlen(buffer),
              "recv", (uint8_t *) g.fs.path, strlen(g.fs.path));
    sprintf(spec, "Network %c online", unit);
    TEST(spec, strcmp(g.fs.path, buffer) == 0);
  }
  TEST("All network devices online", err == FN_ERR_OK);

  // Clear prefix on all devices because it gets prepended to URLs
  for (idx = 0; idx < MAX_NETWORK_DEVICES; idx++) {
    unit = idx + '1';
    sprintf(buffer, "N%c:", unit);
    err = network_fs_cd(buffer);
    if (err != FN_ERR_OK)
      break;
  }
#endif
  END_OF_TEST();
}

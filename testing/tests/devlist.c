#include "broken.h"

#include "devlist.h"
#include "harness.h"
#include "globals.h"

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
  SKIP(FujiNet_online);
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
  SKIP(Clock_online);
#else
  err = clock_get_time(g.clock_fmt, APETIME_BINARY);
  TEST("Clock online", err == FN_ERR_OK);
#endif
  END_OF_TEST();
}

void test_network_exists(void)
{
  uint8_t idx, err;
  char buffer[40];

  SECTION("N: device");

#ifdef FN_BROKEN_network_fs_cd
  SKIP(Network_online);
#else
  for (idx = 0; idx < MAX_NETWORK_DEVICES; idx++) {
    strcpy(buffer, "Nn:/");
    buffer[1] = '1' + idx;
    err = network_fs_cd(buffer);
    strcpy(buffer, "Network n online");
    buffer[8] = '1' + idx;
    TEST(buffer, err == FN_ERR_OK);
  }
#endif
  END_OF_TEST();
}

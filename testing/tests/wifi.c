#include "broken.h"
#include "harness.h"
#include "constants.h"
#include "globals.h"
#include <fujinet-fuji.h>

#ifndef _CMOC_VERSION_
#include <stdio.h>
#include <string.h>
#endif /* _CMOC_VERSION_ */

#define WIFI_CONNECTED 3

/* As CONFIG's connect_wifi() waits: a scan or SET SSID drops WiFi on real
 * hardware, and an IP can take many seconds to come back. */
static bool wait_for_wifi(void)
{
  uint8_t status;
  uint8_t tries;

  for (tries = 0; tries < 20; tries++) {
    status = 0;
    if (fuji_get_wifi_status(&status) && status == WIFI_CONNECTED)
      return true;
    wait_jiffies(150);
  }
  return false;
}

void test_fuji_wifi(void)
{
  bool enabled;
  uint8_t wifi_status;
  bool ok;
  NetConfig nc;

  SECTION("fuji WiFi queries");

#ifdef FN_BROKEN_fuji_get_wifi_enabled
  SKIP(fuji_get_wifi_enabled);
#endif
  enabled = fuji_get_wifi_enabled();
  TEST("fuji_get_wifi_enabled returns true", enabled);

  wifi_status = 0;
#ifdef FN_BROKEN_fuji_get_wifi_status
  SKIP(fuji_get_wifi_status);
#endif
  ok = fuji_get_wifi_status(&wifi_status);
  TEST("fuji_get_wifi_status succeeds", ok);
  TEST("WiFi status is WL_CONNECTED (3)", wifi_status == 3);

  memset(&nc, 0, sizeof(nc));
#ifdef FN_BROKEN_fuji_get_ssid
  SKIP(fuji_get_ssid);
#endif
  ok = fuji_get_ssid(&nc);
  TEST("fuji_get_ssid succeeds", ok);
  TEST("SSID string is non-empty", nc.ssid[0] != '\0');
  printf("  Connected SSID: %s\n", nc.ssid);

  END_OF_TEST();
}

void test_fuji_set_ssid(void)
{
  static NetConfig nc;
  static NetConfig read_back;
  bool saved_read_ok;
  bool credentials_available;
  bool set_ok;
  bool stayed_connected;
  bool read_back_ok;
  bool ssid_matches;
  bool password_matches;
  bool restored;
  bool restore_connected;

  SECTION("fuji_set_ssid");

  memset(&nc, 0, sizeof(nc));
  saved_read_ok = fuji_get_ssid(&nc);
  credentials_available = saved_read_ok && nc.ssid[0] != '\0';
  set_ok = false;
  stayed_connected = false;
  read_back_ok = false;
  ssid_matches = false;
  password_matches = false;
  restored = false;
  restore_connected = false;

  if (credentials_available)
  {
    set_ok = fuji_set_ssid(&nc);
    stayed_connected = wait_for_wifi();

    memset(&read_back, 0, sizeof(read_back));
    read_back_ok = fuji_get_ssid(&read_back);
    ssid_matches = read_back_ok &&
                   memcmp(nc.ssid, read_back.ssid, sizeof(nc.ssid)) == 0;
    password_matches = read_back_ok &&
                       memcmp(nc.password, read_back.password,
                              sizeof(nc.password)) == 0;

    /* Reapply the saved credentials before any TEST can abort the program. */
    restored = fuji_set_ssid(&nc);
    restore_connected = wait_for_wifi();
  }

  TEST("read saved WiFi credentials", saved_read_ok);
  TEST("saved WiFi SSID is available", credentials_available);
  TEST("fuji_set_ssid accepts saved credentials", set_ok);
  TEST("WiFi reconnects after fuji_set_ssid", stayed_connected);
  TEST("read WiFi credentials after fuji_set_ssid", read_back_ok);
  TEST("SSID matches the supplied value", ssid_matches);
  TEST("password matches the supplied value", password_matches);
  TEST("restore call accepts saved WiFi credentials", restored);
  TEST("WiFi is connected after credential restoration", restore_connected);

  END_OF_TEST();
}

void test_fuji_network_scan(void)
{
  uint8_t count;
  bool ok;
  SSIDInfo info;

  SECTION("fuji WiFi scan");

  count = 0;
#ifdef FN_BROKEN_fuji_scan_for_networks
  SKIP(fuji_scan_for_networks);
#endif
  ok = fuji_scan_for_networks(&count);
  TEST("fuji_scan_for_networks succeeds", ok);
  TEST("At least one network visible", count > 0);
  printf("  Networks found: %d\n", count);

  if (count > 0) {
    memset(&info, 0, sizeof(info));
#ifdef FN_BROKEN_fuji_get_scan_result
    SKIP(fuji_get_scan_result);
#endif
    ok = fuji_get_scan_result(0, &info);
    TEST("fuji_get_scan_result(0) succeeds", ok);
    TEST("First scan result SSID non-empty", info.ssid[0] != '\0');
    printf("  First SSID: %s  RSSI: %d\n", info.ssid, (int)info.rssi);
  }

  TEST("WiFi reconnects after the scan", wait_for_wifi());

  END_OF_TEST();
}

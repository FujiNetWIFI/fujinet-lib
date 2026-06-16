#include "broken.h"

#include "harness.h"
#include "constants.h"
#include "globals.h"
#include <fujinet-fuji.h>

#ifndef _CMOC_VERSION_
#include <stdio.h>
#include <string.h>
#endif /* _CMOC_VERSION_ */

void test_fuji_status(void)
{
  FNStatus st, blank;
  bool ok;

  SECTION("fuji_status");

#ifdef FN_BROKEN_fuji_status
  SKIP(fuji_status);
#endif
  memset(&st, 0xFF, sizeof(st));
  memcpy(&blank, &st, sizeof(st));
  ok = fuji_status(&st);
  TEST("fuji_status returns true", ok);
  TEST("fuji_status wrote data into status buffer",
       memcmp(&st, &blank, sizeof(st)) != 0);

  END_OF_TEST();
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

void test_fuji_adapter_config(void)
{
  bool ok;
  bool has_ip;
  bool has_mac;

  SECTION("fuji adapter config");

  memset(&g.adapter.ac, 0, sizeof(g.adapter.ac));
#ifdef FN_BROKEN_fuji_get_adapter_config
  SKIP(fuji_get_adapter_config);
#endif
  ok = fuji_get_adapter_config(&g.adapter.ac);
  TEST("fuji_get_adapter_config succeeds", ok);

  has_ip = (g.adapter.ac.localIP[0] | g.adapter.ac.localIP[1] |
            g.adapter.ac.localIP[2] | g.adapter.ac.localIP[3]) != 0;
  TEST("Adapter has a non-zero local IP", has_ip);

  has_mac = (g.adapter.ac.macAddress[0] | g.adapter.ac.macAddress[1] | g.adapter.ac.macAddress[2] |
             g.adapter.ac.macAddress[3] | g.adapter.ac.macAddress[4] | g.adapter.ac.macAddress[5]) != 0;
  TEST("Adapter has a non-zero MAC address", has_mac);

  TEST("FN version string is non-empty", g.adapter.ac.fn_version[0] != '\0');
  printf("  FN version: %s\n", g.adapter.ac.fn_version);
  printf("  IP: %d.%d.%d.%d\n",
         g.adapter.ac.localIP[0], g.adapter.ac.localIP[1],
         g.adapter.ac.localIP[2], g.adapter.ac.localIP[3]);

  memset(&g.adapter.ace, 0, sizeof(g.adapter.ace));
#ifdef FN_BROKEN_fuji_get_adapter_config_extended
  SKIP(fuji_get_adapter_config_extended);
#endif
  ok = fuji_get_adapter_config_extended(&g.adapter.ace);
  TEST("fuji_get_adapter_config_extended succeeds", ok);
  TEST("Extended config has sLocalIP string", g.adapter.ace.sLocalIP[0] != '\0');
  TEST("Extended config has sMacAddress string", g.adapter.ace.sMacAddress[0] != '\0');
  printf("  sLocalIP: %s  sMac: %s\n", g.adapter.ace.sLocalIP, g.adapter.ace.sMacAddress);

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

  END_OF_TEST();
}

void test_fuji_host_device_slots(void)
{
  bool ok;
  bool write_ok;

  SECTION("fuji host and device slots");

  memset(g.slots.hosts,   0, sizeof(g.slots.hosts));
  memset(g.slots.devices, 0, sizeof(g.slots.devices));

#ifdef FN_BROKEN_fuji_get_host_slots
  SKIP(fuji_get_host_slots);
#else
  ok = fuji_get_host_slots(&g.slots.hosts[0], MAX_HOSTS);
  TEST("fuji_get_host_slots succeeds", ok);
  printf("  Host slot 0: %s\n", (char *)g.slots.hosts[0]);
#endif

#ifdef FN_BROKEN_fuji_get_device_slots
  SKIP(fuji_get_device_slots);
#endif
  ok = fuji_get_device_slots(g.slots.devices, MAX_DISKS);
  TEST("fuji_get_device_slots succeeds", ok);

#ifdef FN_BROKEN_fuji_put_host_slots
  SKIP(fuji_put_host_slots);
#else
  write_ok = fuji_put_host_slots(&g.slots.hosts[0], MAX_HOSTS);
  TEST("fuji_put_host_slots round-trip succeeds", write_ok);

  memset(g.slots.hosts2, 0, sizeof(g.slots.hosts2));
  ok = fuji_get_host_slots(&g.slots.hosts2[0], MAX_HOSTS);
  TEST("Re-read host slots after write succeeds", ok);
  TEST("Host slot data unchanged after write-back",
       memcmp(g.slots.hosts, g.slots.hosts2, sizeof(g.slots.hosts)) == 0);
#endif

  END_OF_TEST();
}

void test_fuji_host_prefix(void)
{
  bool ok;

  SECTION("fuji host prefix");

  memset(g.host_prefix.prefix,    0, sizeof(g.host_prefix.prefix));
  memset(g.host_prefix.read_back, 0, sizeof(g.host_prefix.read_back));

#ifdef FN_BROKEN_fuji_get_host_prefix
  SKIP(fuji_get_host_prefix);
#endif
  ok = fuji_get_host_prefix(0, g.host_prefix.prefix);
  TEST("fuji_get_host_prefix succeeds", ok);
  printf("  Current prefix slot 0: '%s'\n", g.host_prefix.prefix);

#ifdef FN_BROKEN_fuji_set_host_prefix
  SKIP(fuji_set_host_prefix);
#endif
  ok = fuji_set_host_prefix(0, (char *) "/test_prefix");
  TEST("fuji_set_host_prefix succeeds", ok);

  ok = fuji_get_host_prefix(0, g.host_prefix.read_back);
  TEST("fuji_get_host_prefix reads back set value", ok);
  TEST("Set prefix matches read-back value", strcmp(g.host_prefix.read_back, "/test_prefix") == 0);

  /* Restore original */
  fuji_set_host_prefix(0, g.host_prefix.prefix);

  END_OF_TEST();
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
#endif
#endif

  END_OF_TEST();
}

void test_fuji_directory_ops(void)
{
  bool ok;
  char entry[64];
  uint16_t pos;
  uint16_t pos_after_rewind;

  SECTION("fuji directory operations (requires mounted host slot 0)");

#ifdef FN_BROKEN_fuji_mount_host_slot
  SKIP(fuji_mount_host_slot);
#endif
  ok = fuji_mount_host_slot(0);
  TEST("fuji_mount_host_slot(0) succeeds", ok);

  if (!ok) {
    printf("  SKIP: host slot 0 unavailable\n");
    goto fn_skipped;
  }

  memset(g.dir, 0, sizeof(g.dir));
  g.dir[0] = '/';

#ifdef FN_BROKEN_fuji_open_directory
  SKIP(fuji_open_directory);
#endif
  ok = fuji_open_directory(0, g.dir);
  TEST("fuji_open_directory('/') succeeds", ok);

#ifdef FN_BROKEN_fuji_read_directory
  SKIP(fuji_read_directory);
#endif
  memset(entry, 0, sizeof(entry));
  ok = fuji_read_directory(sizeof(entry) - 1, 0, entry);
  TEST("fuji_read_directory (first entry) succeeds", ok);
  printf("  First directory entry: %s\n", entry);

#ifdef FN_BROKEN_fuji_get_directory_position
  SKIP(fuji_get_directory_position);
#endif
  pos = 0;
  ok = fuji_get_directory_position(&pos);
  TEST("fuji_get_directory_position succeeds", ok);
  printf("  Directory position after 1 read: %u\n", pos);

#ifdef FN_BROKEN_fuji_set_directory_position
  SKIP(fuji_set_directory_position);
#endif
  ok = fuji_set_directory_position(0);
  TEST("fuji_set_directory_position(0) succeeds", ok);

  pos_after_rewind = 0xFFFF;
  fuji_get_directory_position(&pos_after_rewind);
  TEST("Directory position is 0 after rewind", pos_after_rewind == 0);

#ifdef FN_BROKEN_fuji_close_directory
  SKIP(fuji_close_directory);
#endif
  ok = fuji_close_directory();
  TEST("fuji_close_directory succeeds", ok);

#ifdef FN_BROKEN_fuji_open_directory2
  SKIP(fuji_open_directory2);
#endif
  ok = fuji_open_directory2(0, (char *) "/", NULL);
  TEST("fuji_open_directory2('/', NULL) succeeds", ok);

  ok = fuji_close_directory();
  TEST("fuji_close_directory (second open) succeeds", ok);

  END_OF_TEST();
}

void test_fuji_copy_file(void)
{
  bool ok;
  uint8_t i;
  uint8_t src_idx;
  uint8_t sd_idx;
  bool found_src;
  bool found_sd;
  uint8_t *slot;
  uint8_t saved_src[32];
  char copy_spec[40];
  unsigned long src_size;
  unsigned long dest_size;
  bool copy_ok;
  bool file_existed;
  uint8_t pre_day;
  uint8_t pre_min;
  uint8_t scan;

  SECTION("fuji_copy_file (tnfs.fujinet.online COCO/lobby.dsk -> SD)");

  src_idx = MAX_HOSTS - 2;
  sd_idx = 0;
  found_src = false;
  found_sd = false;
  src_size = 0;
  dest_size = 0;
  copy_ok = false;
  file_existed = false;
  pre_day = 0;
  pre_min = 0;
  memset(saved_src, 0, sizeof(saved_src));

#ifdef FN_BROKEN_fuji_get_host_slots
  SKIP(fuji_get_host_slots);
#else

  ok = fuji_get_host_slots(&g.slots.hosts[0], MAX_HOSTS);
  TEST("fuji_get_host_slots succeeds", ok);

  /* CMOC: g.slots.hosts[i][j] strides by 1 byte; use pointer decay instead. */
  for (i = 0; i < MAX_HOSTS; i++) {
    slot = (uint8_t *)g.slots.hosts[i];
    if (slot[0] == '\0' && !found_src) {
      src_idx = i;
      found_src = true;
    }
  }

  for (i = 0; i < MAX_HOSTS; i++) {
    slot = (uint8_t *)g.slots.hosts[i];
    if (slot[0] == 'S' && slot[1] == 'D' && slot[2] == '\0') {
      sd_idx = i;
      found_sd = true;
      break;
    }
  }
  TEST("SD host slot found", found_sd);
  printf("  src slot %u  sd slot %u\n",
         (unsigned)(src_idx + 1), (unsigned)(sd_idx + 1));

  slot = (uint8_t *)g.slots.hosts[src_idx];
  memcpy(saved_src, slot, 32);
  memset(slot, 0, 32);
  strcpy((char *)slot, "tnfs.fujinet.online");

  ok = fuji_put_host_slots(&g.slots.hosts[0], MAX_HOSTS);
  TEST("fuji_put_host_slots (with source) succeeds", ok);

  ok = fuji_mount_host_slot(src_idx);
  TEST("fuji_mount_host_slot (TNFS) succeeds", ok);

  ok = fuji_mount_host_slot(sd_idx);
  TEST("fuji_mount_host_slot (SD) succeeds", ok);

  /* Check if lobby.dsk already exists on SD and record its timestamp.
   * Use a local buffer so fuji_read_directory does not corrupt g.slots.hosts
   * (which aliases g.dir in the global union). */
  {
    uint8_t entry[49];
    memset(g.dir, 0, MAX_FILENAME_LEN);
    strcpy(g.dir, "/");
    if (fuji_open_directory(sd_idx, g.dir)) {
      for (scan = 0; scan < 64; scan++) {
        memset(entry, 0, 49);
        fuji_read_directory(49, 0x80, entry);
        if ((unsigned char)entry[0] == 0x7F)
          break;
        if (strcmp((char *)entry + 13, "lobby.dsk") == 0) {
          file_existed = true;
          pre_day = entry[2];
          pre_min = entry[4];
          printf("  Pre-copy lobby.dsk: day=%u min=%u\n", (unsigned)pre_day, (unsigned)pre_min);
          break;
        }
      }
      fuji_close_directory();
    }
    if (!file_existed)
      printf("  lobby.dsk not found on SD (will be created by copy)\n");
  }

  strcpy(g.dir, "/COCO/");
  strcpy(g.dir + 7, "lobby.dsk");
  ok = fuji_open_directory(src_idx, g.dir);
  TEST("fuji_open_directory TNFS /COCO/lobby.dsk succeeds", ok);
  if (ok) {
    fuji_read_directory(49, 0x80, g.dir);
    if ((unsigned char)g.dir[0] != 0x7F) {
      memcpy(&src_size, (uint8_t *)g.dir + 6, sizeof(unsigned long));
      printf("  Source size: %lu bytes\n", src_size);
    }
    fuji_close_directory();
  }
  TEST("lobby.dsk found on TNFS source (size non-zero)", src_size > 0);

  /* copy_spec is a local buffer — g.slots.hosts aliases g.dir in the union */
  strcpy(copy_spec, "COCO/lobby.dsk|lobby.dsk");
  printf("  fuji_copy_file(src=%u, dst=%u, \"%s\")\n",
         (unsigned)(src_idx + 1), (unsigned)(sd_idx + 1), copy_spec);
  copy_ok = fuji_copy_file(src_idx + 1, sd_idx + 1, copy_spec);

  /* Re-read first — g.slots.hosts/g.dir alias in the union. */
  fuji_get_host_slots(&g.slots.hosts[0], MAX_HOSTS);
  slot = (uint8_t *)g.slots.hosts[src_idx];
  memcpy(slot, saved_src, 32);
  fuji_put_host_slots(&g.slots.hosts[0], MAX_HOSTS);
  fuji_mount_host_slot(sd_idx);

  TEST("fuji_copy_file (COCO/lobby.dsk -> SD lobby.dsk) succeeds", copy_ok);

  memset(g.dir, 0, MAX_FILENAME_LEN);
  strcpy(g.dir, "/");
  ok = fuji_open_directory(sd_idx, g.dir);
  TEST("fuji_open_directory SD / succeeds", ok);
  if (ok) {
    /* SDFS doesn't filter at open time; scan until we find lobby.dsk or EOF. */
    for (scan = 0; scan < 64; scan++) {
      memset(g.dir, 0, 49);
      fuji_read_directory(49, 0x80, g.dir);
      if ((unsigned char)g.dir[0] == 0x7F)
        break;
      if (strcmp(g.dir + 13, "lobby.dsk") == 0) {
        memcpy(&dest_size, (uint8_t *)g.dir + 6, sizeof(unsigned long));
        printf("  Dest size:   %lu bytes  day=%u min=%u\n",
               dest_size, (unsigned)(uint8_t)g.dir[2], (unsigned)(uint8_t)g.dir[4]);
        break;
      }
    }
    fuji_close_directory();
  }
  TEST("lobby.dsk found on SD after copy (size non-zero)", dest_size > 0);
  TEST("dest lobby.dsk size matches source", dest_size == src_size);
  if (file_existed) {
    TEST("lobby.dsk timestamp changed after copy",
         (uint8_t)g.dir[2] != pre_day || (uint8_t)g.dir[4] != pre_min);
  }

#endif /* FN_BROKEN_fuji_get_host_slots */

  END_OF_TEST();
}

#include "broken.h"

#include "harness.h"
#include "constants.h"
#include "globals.h"
#include <fujinet-fuji.h>

#ifndef _CMOC_VERSION_
#include <stdio.h>
#include <string.h>
#endif /* _CMOC_VERSION_ */

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

#ifdef FN_BROKEN_fuji_copy_file
  SKIP(fuji_copy_file);
#else

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
#endif /* FN_BROKEN_fuji_copy_file */

  END_OF_TEST();
}

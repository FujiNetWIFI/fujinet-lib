#include "broken.h"

#include "harness.h"
#include "constants.h"
#include "globals.h"
#include <fujinet-fuji.h>
#include <fujinet-network.h>

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
  bool src_preexisting;
  uint8_t *slot;
  uint8_t saved_src[32];
  char copy_spec[40];
  unsigned long src_size;
  unsigned long dest_size;
  bool copy_ok;
  bool file_existed;
  uint8_t pre_day;
  uint8_t pre_min;
  uint8_t post_day;
  uint8_t post_min;
  FujiDirEntry *e;
  /* Not g.dslots, which overlays g.hslots. */
  static DeviceSlot dslots[MAX_DISKS];
  uint8_t in_use;

  SECTION("fuji_copy_file (tnfs.fujinet.online COCO/lobby.dsk -> SD)");

  src_idx = 0;
  sd_idx = 0;
  found_src = false;
  found_sd = false;
  src_preexisting = false;
  src_size = 0;
  dest_size = 0;
  copy_ok = false;
  file_existed = false;
  pre_day = 0;
  pre_min = 0;
  post_day = 0;
  post_min = 0;
  memset(saved_src, 0, sizeof(saved_src));

#ifdef FN_BROKEN_fuji_copy_file
  SKIP(fuji_copy_file);
#else

#ifdef FN_BROKEN_fuji_get_host_slots
  SKIP(fuji_get_host_slots);
#else
  ok = fuji_get_host_slots(&g.hslots.hosts[0], MAX_HOSTS);
  TEST("fuji_get_host_slots succeeds", ok);

  for (i = 0; i < MAX_HOSTS; i++) {
    slot = (uint8_t *)g.hslots.hosts[i];
    if (slot[0] == 'S' && slot[1] == 'D' && slot[2] == '\0') {
      sd_idx = i;
      found_sd = true;
      break;
    }
  }
  TEST("SD host slot found", found_sd);

  /* Host slots that mounted disks read from. Renaming one pulls the host
   * out from under the disk (fujinet-firmware#1983). */
  in_use = 0;
  if (fuji_get_device_slots(dslots, MAX_DISKS)) {
    for (i = 0; i < MAX_DISKS; i++)
      if (dslots[i].file[0] && dslots[i].hostSlot < MAX_HOSTS)
        in_use |= (uint8_t)(1 << dslots[i].hostSlot);
  }

  /* CMOC: g.hslots.hosts[i][j] strides by 1 byte; use pointer decay instead.
   * Prefer a host slot already set to tnfs.fujinet.online, then an empty host
   * slot, then any other host slot no mounted disk reads from. A borrowed host
   * slot's name is put back afterward. */
  for (i = 0; i < MAX_HOSTS; i++) {
    slot = (uint8_t *)g.hslots.hosts[i];
    if (strncmp((char *)slot, TNFS_COPY_SOURCE_HOST, 32) == 0) {
      src_idx = i;
      found_src = true;
      src_preexisting = true;
      break;
    }
  }

  if (!found_src) {
    for (i = 0; i < MAX_HOSTS; i++) {
      slot = (uint8_t *)g.hslots.hosts[i];
      if (slot[0] == '\0') {
        src_idx = i;
        found_src = true;
        break;
      }
    }
  }

  if (!found_src) {
    for (i = 0; i < MAX_HOSTS; i++) {
      if (i != sd_idx && !(in_use & (1 << i))) {
        src_idx = i;
        found_src = true;
        break;
      }
    }
  }

  if (!found_src) {
    printf("  every host slot is in use by a mounted disk\n");
    SKIP(fuji_copy_file);
  }

  printf("  src slot %u  sd slot %u  (%s)\n",
         (unsigned)(src_idx + 1), (unsigned)(sd_idx + 1),
         src_preexisting ? "already present" : "temporary");

  if (!src_preexisting) {
    slot = (uint8_t *)g.hslots.hosts[src_idx];
    memcpy(saved_src, slot, 32);
    memset(slot, 0, 32);
    strcpy((char *)slot, TNFS_COPY_SOURCE_HOST);

    ok = fuji_put_host_slots(&g.hslots.hosts[0], MAX_HOSTS);
    TEST("fuji_put_host_slots (with source) succeeds", ok);
  }

  ok = fuji_mount_host_slot(src_idx);
  TEST("fuji_mount_host_slot (TNFS) succeeds", ok);

  ok = fuji_mount_host_slot(sd_idx);
  TEST("fuji_mount_host_slot (SD) succeeds", ok);

  /* fuji_file_stat() decodes the directory entry for each platform. */
  e = fuji_file_stat(sd_idx, "/lobby.dsk");
  if (e) {
    file_existed = true;
    pre_day = e->modified.day;
    pre_min = e->modified.minute;
    printf("  Pre-copy lobby.dsk: day=%u min=%u\n", (unsigned)pre_day, (unsigned)pre_min);
  }
  else
    printf("  lobby.dsk not found on SD (will be created by copy)\n");

  e = fuji_file_stat(src_idx, "/COCO/lobby.dsk");
  TEST("lobby.dsk found on the TNFS source", e != NULL);
  if (e) {
    src_size = e->size;
    printf("  Source size: %lu bytes\n", src_size);
  }
  TEST("lobby.dsk on the TNFS source is not empty", src_size > 0);

  strcpy(copy_spec, "COCO/lobby.dsk|lobby.dsk");
  printf("  fuji_copy_file(src=%u, dst=%u, \"%s\")\n",
         (unsigned)(src_idx + 1), (unsigned)(sd_idx + 1), copy_spec);
  copy_ok = fuji_copy_file(src_idx + 1, sd_idx + 1, copy_spec);

  if (!src_preexisting) {
    /* Put the borrowed host slot back as it was. */
    ok = fuji_get_host_slots(&g.hslots.hosts[0], MAX_HOSTS);
    TEST("fuji_get_host_slots (restore re-read) succeeds", ok);
    if (ok) {
      slot = (uint8_t *)g.hslots.hosts[src_idx];
      memcpy(slot, saved_src, 32);
      fuji_put_host_slots(&g.hslots.hosts[0], MAX_HOSTS);
    }
  }
  fuji_mount_host_slot(sd_idx);

  TEST("fuji_copy_file (COCO/lobby.dsk -> SD lobby.dsk) succeeds", copy_ok);

  e = fuji_file_stat(sd_idx, "/lobby.dsk");
  TEST("lobby.dsk found on SD after copy", e != NULL);
  if (e) {
    dest_size = e->size;
    post_day = e->modified.day;
    post_min = e->modified.minute;
    printf("  Dest size:   %lu bytes  day=%u min=%u\n",
           dest_size, (unsigned)post_day, (unsigned)post_min);
  }
  TEST("dest lobby.dsk size matches source", dest_size == src_size);
  if (file_existed) {
    TEST("lobby.dsk timestamp changed after copy",
         post_day != pre_day || post_min != pre_min);
  }

#endif /* FN_BROKEN_fuji_get_host_slots */
#endif /* FN_BROKEN_fuji_copy_file */

  END_OF_TEST();
}

#define NEW_DISK_NAME "/fnnewdisk.img"
#define NEW_DISK_URL  "N1:SD://" NEW_DISK_NAME

void test_fuji_create_new(void)
{
#ifdef FN_BROKEN_fuji_create_new
  SECTION("fuji_create_new");
  SKIP(fuji_create_new);
#else
  static NewDisk nd;
  /* Not g.dslots, which overlays g.hslots. */
  static DeviceSlot saved[MAX_DISKS];
  uint8_t i, sd_idx, ds;
  uint8_t *slot;
  bool ok, found_sd;
  unsigned long want;
  FujiDirEntry *e;

  SECTION("fuji_create_new");

  ok = fuji_get_host_slots(&g.hslots.hosts[0], MAX_HOSTS);
  TEST("fuji_get_host_slots succeeds", ok);
  found_sd = false;
  sd_idx = 0;
  for (i = 0; i < MAX_HOSTS; i++) {
    slot = (uint8_t *)g.hslots.hosts[i];
    if (slot[0] == 'S' && slot[1] == 'D' && slot[2] == '\0') {
      sd_idx = i;
      found_sd = true;
      break;
    }
  }
  TEST("SD host slot found", found_sd);

  ok = fuji_get_device_slots(saved, MAX_DISKS);
  TEST("fuji_get_device_slots succeeds", ok);
  for (ds = 0; ds < MAX_DISKS && saved[ds].file[0]; ds++)
    ;
  if (ds == MAX_DISKS) {
    printf("  every drive slot is in use\n");
    SKIP(fuji_create_new);
  }
  printf("  drive slot %u, SD host slot %u\n", (unsigned)(ds + 1), (unsigned)(sd_idx + 1));

  ok = fuji_mount_host_slot(sd_idx);
  TEST("fuji_mount_host_slot (SD) succeeds", ok);
  /* A stopped run leaves the image behind, and NEW DISK refuses to replace it */
  network_fs_delete(NEW_DISK_URL);

  memset(&nd, 0, sizeof(nd));
  nd.hostSlot = sd_idx;
  nd.deviceSlot = ds;
  strcpy(nd.filename, NEW_DISK_NAME);
#if defined(FUJI_PLATFORM_APPLE2)
  nd.createType = NEW_DISK_TYPE_PO;
  nd.numBlocks = 280;
  want = 280UL * 512;
#elif defined(FUJI_PLATFORM_COCO) || defined(FUJI_PLATFORM_PMD85)
  nd.numDisks = 1;
  want = 315UL * 512;
#elif defined(FUJI_PLATFORM_LYNX)
  nd.numBlocks = 256;
  want = 256UL * 1024;
#else
  nd.numSectors = 720;
  nd.sectorSize = NEW_DISK_SECTOR_SINGLE;
  want = 720UL * NEW_DISK_SECTOR_SINGLE;
#ifdef FUJI_PLATFORM_ATARI
  want += 16; /* ATR header */
#endif
#endif

  ok = fuji_create_new(&nd);
  TEST("fuji_create_new succeeds", ok);

  e = fuji_file_stat(sd_idx, NEW_DISK_NAME);
  TEST("the new image is on the SD host", e != NULL);
  if (e) {
    printf("  size %lu, %lu expected\n", (unsigned long) e->size, want);
    TEST("the new image is the size asked for", e->size == want);
  }

  ok = fuji_create_new(&nd);
  TEST("creating it again over the existing file fails", !ok);

  /* NEW DISK points the drive slot at the image; put it back */
  fuji_set_device_filename(saved[ds].mode, saved[ds].hostSlot, ds, (char *) saved[ds].file);
  TEST("the image is deleted", network_fs_delete(NEW_DISK_URL) == FN_ERR_OK);
#endif /* FN_BROKEN_fuji_create_new */

  END_OF_TEST();
}

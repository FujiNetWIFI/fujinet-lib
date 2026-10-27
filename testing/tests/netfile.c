#include "broken.h"

#include "harness.h"
#include "constants.h"
#include "globals.h"
#include "cmp_hex.h"
#include <fujinet-fuji.h>
#include <fujinet-network.h>

#ifndef _CMOC_VERSION_
#include <stdio.h>
#include <string.h>
#endif /* _CMOC_VERSION_ */

/* These tests are built on primitives that are unavailable on some
 * platforms with fujinet-lib 4.x, leaving nothing to exercise. */
#if defined(FN_BROKEN_network_open) || defined(FN_BROKEN_network_close) \
 || defined(FN_BROKEN_network_read) || defined(FN_BROKEN_network_http_put)
#define FN_BROKEN_network_fs
#endif

#ifndef FN_BROKEN_network_fs

/* IWM acknowledges every open at the bus level and reports the real result
 * through status, so network_open alone always looks successful there. */
#ifdef BUILD_APPLE2
static uint8_t fs_open_result(const char *devicespec, uint8_t err)
{
  uint16_t bw;
  uint8_t conn;
  uint8_t nerr;

  if (err != FN_ERR_OK)
    return err;

  if (network_status(devicespec, &bw, &conn, &nerr) != FN_ERR_OK)
    return FN_ERR_IO_ERROR;

  /* A directory reports EOF until a read pulls the first entry, so that
   * is not a failure of the open. */
  if (nerr == NETWORK_ERROR_END_OF_FILE)
    nerr = NETWORK_SUCCESS;

  if (nerr > NETWORK_SUCCESS && !bw)
    return nerr;

  return err;
}
#else
#define fs_open_result(devicespec, err) (err)
#endif /* BUILD_APPLE2 */

/* Entries end with CRLF, or 0x9B on the Atari. */
#define DIR_IS_TERMINATOR(c) ((c) == '\r' || (c) == '\n' || (c) == 0x9B)

/* Create or replace a file. The PUT is not sent until the close. */
static uint8_t fs_put(const char *devicespec, const char *data)
{
  uint8_t err;

  err = network_open(devicespec, OPEN_MODE_HTTP_PUT, OPEN_TRANS_NONE);
  if (err != FN_ERR_OK)
    return err;

  err = network_http_put(devicespec, data);
  if (err != FN_ERR_OK) {
    network_close(devicespec);
    return err;
  }

  return network_close(devicespec);
}

/* Build g.fs.path as <base><leaf>. */
static void fs_path(const char *leaf)
{
  memset(g.fs.path, 0, sizeof(g.fs.path));
  strcpy(g.fs.path, g.fs.base);
  strcat(g.fs.path, leaf);
}

static int16_t fs_listing_len;

/* Read a directory into g.fs.listing, printing each entry. Returns the
 * number of entries seen, or -1 if the directory would not open. */
static int16_t fs_list(const char *devicespec, uint8_t mode, uint8_t format)
{
  int16_t r;
  int16_t count = 0;
  uint16_t i;
  uint8_t err;

  err = fs_open_result(devicespec, network_open(devicespec, mode, format));
  if (err != FN_ERR_OK)
    return -1;

  memset(g.fs.listing, 0, sizeof(g.fs.listing));
  r = network_read(devicespec, (uint8_t *)g.fs.listing, sizeof(g.fs.listing) - 1);
  network_close(devicespec);

  fs_listing_len = r > 0 ? r : 0;
  if (r <= 0)
    return 0;

  /* Split on the line ending so each name can be printed and matched. */
  for (i = 0; i < (uint16_t)r; i++) {
    if (DIR_IS_TERMINATOR((uint8_t)g.fs.listing[i]))
      g.fs.listing[i] = '\0';
  }

  for (i = 0; i < (uint16_t)r; ) {
    if (g.fs.listing[i]) {
      printf("    %s\n", &g.fs.listing[i]);
      count++;
      i += strlen(&g.fs.listing[i]);
    }
    i++;
  }

  return count;
}

/* True if name appears in the listing left by fs_list(). */
static bool fs_listed(const char *name)
{
  uint16_t i;

  for (i = 0; i < (uint16_t)fs_listing_len; ) {
    if (g.fs.listing[i]) {
      if (strcmp(&g.fs.listing[i], name) == 0)
        return true;
      i += strlen(&g.fs.listing[i]);
    }
    i++;
  }

  return false;
}

/* Read a whole file into g.fs.data. Returns bytes read, -1 if it would
 * not open. The GET is issued by the status call inside network_read. */
static int16_t fs_get(const char *devicespec)
{
  int16_t r;

  if (network_open(devicespec, OPEN_MODE_HTTP_GET, OPEN_TRANS_NONE) != FN_ERR_OK)
    return -1;

  memset(g.fs.data, 0, sizeof(g.fs.data));
  r = network_read(devicespec, (uint8_t *)g.fs.data, sizeof(g.fs.data) - 1);
  network_close(devicespec);

  return r;
}

#endif /* ! FN_BROKEN_network_fs */

void test_fs_make_test_dir(void)
{
  char guid[MAX_GUID_LEN];
  uint8_t err;
  bool ok;

  SECTION("network_fs_mkdir");

#ifdef FN_BROKEN_network_fs
  SKIP(network_fs);
  END_OF_TEST();
#else
#ifdef FN_BROKEN_network_init
  SKIP(network_init);
#else
  err = network_init();
  TEST("network_init returns FN_ERR_OK", err == FN_ERR_OK);
#endif

#ifdef FN_BROKEN_fuji_generate_guid
  SKIP(fuji_generate_guid);
#else
  memset(guid, 0, sizeof(guid));
  ok = fuji_generate_guid(guid);
  TEST("fuji_generate_guid succeeds", ok);
  TEST("GUID length is 36", strlen(guid) == 36);
#endif

  memset(g.fs.base, 0, sizeof(g.fs.base));
  strcpy(g.fs.base, WEBDAV_ROOT);
  strcat(g.fs.base, guid);
  strcat(g.fs.base, "/");
  printf("  test dir: %s\n", g.fs.base);

  /* An earlier run that stopped before its teardown leaves this directory
   * behind, and mkdir on an existing one fails. Not an error if absent. */
  err = network_fs_rmdir(g.fs.base);
  network_close(g.fs.base);

#ifdef FN_BROKEN_network_fs_mkdir
  SKIP(network_fs_mkdir);
#else
  err = network_fs_mkdir(g.fs.base);
  TEST("network_fs_mkdir returns FN_ERR_OK", err == FN_ERR_OK);
#endif

  /* A WebDAV directory open only succeeds on a collection that is really
   * there, so this is what proves the mkdir took effect. */
  err = fs_open_result(g.fs.base,
                       network_open(g.fs.base, OPEN_MODE_DIRECTORY, DIR_FORMAT_RAW));
  TEST("created directory opens for listing", err == FN_ERR_OK);
  network_close(g.fs.base);

  /* Without this the check above would pass even if the open succeeded
   * unconditionally. */
  memset(g.fs.path, 0, sizeof(g.fs.path));
  strcpy(g.fs.path, WEBDAV_ROOT);
  strcat(g.fs.path, "fnfstest-never-created/");
  err = fs_open_result(g.fs.path,
                       network_open(g.fs.path, OPEN_MODE_DIRECTORY, DIR_FORMAT_RAW));
  TEST("directory that was never created does not open", err != FN_ERR_OK);
  network_close(g.fs.path);

  END_OF_TEST();
#endif
}

void test_fs_prefix(void)
{
  char guid[MAX_GUID_LEN + 1];
  uint8_t err;
  char *r;

  SECTION("network_fs_cd");

#if defined(FN_BROKEN_network_fs_cd) || defined(FN_BROKEN_network_fs_pwd)
#ifdef FN_BROKEN_network_fs_cd
  SKIP(network_fs_cd);
#endif
#ifdef FN_BROKEN_network_fs_pwd
  SKIP(network_fs_pwd);
#endif
#else
  fuji_generate_guid(&guid[1]);
  guid[0] = '/';

#if 0
  // Nothing actually checks if the directory exists when doing a cd
  strcpy(g.fs.base, WEBDAV_ROOT);
  strcat(g.fs.base, guid);
  strcat(g.fs.base, "/");
  printf("  test dir: %s\n", g.fs.base);

  err = network_fs_mkdir(g.fs.base);
  TEST("directory created for cd", err == FN_ERR_OK);
#else
  strcpy(g.fs.base, "N1:");
  strcat(g.fs.base, guid);
#endif

  // An aborted run may have left a prefix set
  network_fs_cd("N1:");
  err = network_fs_cd(g.fs.base);
  TEST("change directory succeeds", err == FN_ERR_OK);

  err = network_fs_pwd(g.fs.base, g.fs.path);
  TEST("getcwd succeeds", err == FN_ERR_OK);
  r = strrchr(g.fs.path, '/');
  if (r && r > &g.fs.path[1])
    *r = 0;
  if (strcmp(g.fs.path, guid) != 0)
    cmp_hex("orig", (uint8_t *) guid, strlen(guid),
            "recv", (uint8_t *) g.fs.path, strlen(g.fs.path));
  TEST("working directory matches", strcmp(g.fs.path, guid) == 0);

#ifndef FN_BROKEN_network_fs
  /* A full URL replaces the prefix rather than being appended to it
   * (fujinet-firmware#1735). */
  err = fs_open_result(NET_DEVICESPEC,
                       network_open(NET_DEVICESPEC, OPEN_MODE_HTTP_GET, OPEN_TRANS_NONE));
  TEST("a full URL opens while a prefix is set", err == FN_ERR_OK);
  TEST("the full URL returns data",
       network_read(NET_DEVICESPEC, (uint8_t *)g.fs.listing, sizeof(g.fs.listing)) > 0);
  network_close(NET_DEVICESPEC);
#endif

  // Reset prefix to empty otherwise it is prepended to relative specs
  network_fs_cd("N1:");
#endif

  END_OF_TEST();
}

void test_fs_create_files(void)
{
  int16_t count;
  uint8_t err;

  SECTION("network_fs: subdirectory and files");

#ifdef FN_BROKEN_network_fs
  SKIP(network_fs);
  END_OF_TEST();
#else
  fs_path("sub/");
#ifdef FN_BROKEN_network_fs_mkdir
  SKIP(network_fs_mkdir);
#else
  err = network_fs_mkdir(g.fs.path);
  TEST("mkdir of a second level directory returns FN_ERR_OK", err == FN_ERR_OK);
#endif

  err = fs_open_result(g.fs.path,
                       network_open(g.fs.path, OPEN_MODE_DIRECTORY, DIR_FORMAT_RAW));
  TEST("subdirectory opens for listing", err == FN_ERR_OK);
  network_close(g.fs.path);

  fs_path("sub/alpha.txt");
  TEST("alpha.txt is created", fs_put(g.fs.path, "alpha") == FN_ERR_OK);

  fs_path("sub/beta.txt");
  TEST("beta.txt is created", fs_put(g.fs.path, "beta") == FN_ERR_OK);

  fs_path("sub/gamma.txt");
  TEST("gamma.txt is created", fs_put(g.fs.path, "gamma") == FN_ERR_OK);

  fs_path("sub/delta.txt");
  TEST("delta.txt is created", fs_put(g.fs.path, "delta") == FN_ERR_OK);

  fs_path("sub/");
  printf("  contents of sub/\n");
  count = fs_list(g.fs.path, OPEN_MODE_DIRECTORY, DIR_FORMAT_RAW);
  TEST("directory listing is not empty", count > 0);
  TEST("alpha.txt is listed", fs_listed("alpha.txt"));
  TEST("beta.txt is listed", fs_listed("beta.txt"));
  TEST("gamma.txt is listed", fs_listed("gamma.txt"));
  TEST("delta.txt is listed", fs_listed("delta.txt"));

  END_OF_TEST();
#endif
}

void test_fs_directory_modes(void)
{
  int16_t count;
  uint8_t err;

  SECTION("network_fs: directory modes and formats");

#ifdef FN_BROKEN_network_fs
  SKIP(network_fs);
  END_OF_TEST();
#else
  fs_path("sub/");

  /* The translation is sticky and ORed into later opens' aux2, which must
   * not happen to a directory open, where aux2 is the format
   * (fujinet-firmware#1584). CRLF (3) turns LONG (0x80) into RAW (0x83),
   * so a leak shows up as the bare filename being listed. */
  err = network_set_translation(g.fs.path, OPEN_TRANS_CRLF);
  TEST("network_set_translation returns FN_ERR_OK", err == FN_ERR_OK);

  printf("  contents of sub/, DIRECTORY, LONG\n");
  count = fs_list(g.fs.path, OPEN_MODE_DIRECTORY, DIR_FORMAT_LONG);
  TEST("DIRECTORY lists in LONG format", count > 0);
  TEST("DIRECTORY keeps LONG after a translation is set", !fs_listed("alpha.txt"));

  printf("  contents of sub/, DIRECTORY_ALT, LONG\n");
  count = fs_list(g.fs.path, OPEN_MODE_DIRECTORY_ALT, DIR_FORMAT_LONG);
  TEST("DIRECTORY_ALT lists in LONG format", count > 0);
  TEST("DIRECTORY_ALT keeps LONG after a translation is set", !fs_listed("alpha.txt"));

  printf("  contents of sub/, DIRECTORY_ALT, RAW\n");
  count = fs_list(g.fs.path, OPEN_MODE_DIRECTORY_ALT, DIR_FORMAT_RAW);
  TEST("DIRECTORY_ALT lists in RAW format", count > 0);
  TEST("DIRECTORY_ALT RAW listing has the exact name", fs_listed("alpha.txt"));

  network_set_translation(g.fs.path, OPEN_TRANS_NONE);

  END_OF_TEST();
#endif
}

/* Firmware once took the LONG format byte for a translation mode and read
 * ahead only 512 bytes per STATUS; a client trusting the first STATUS, as
 * Atari NOS does, lost the rest (fujinet-firmware#1836). */
void test_fs_long_listing(void)
{
#ifndef FN_BROKEN_network_fs
  char chunk[64];
  int16_t r, i;
  uint16_t total = 0, entries = 0, col = 0;
  char first = 0;
  bool saw_z = false;
  uint8_t err, conn, nerr;
  uint16_t bw;
#endif

  SECTION("network_fs: LONG listing over 512 bytes");

#ifdef FN_BROKEN_network_fs
  SKIP(network_fs);
#else
  err = fs_open_result(TNFS_BIG_DIR,
                       network_open(TNFS_BIG_DIR, OPEN_MODE_DIRECTORY, DIR_FORMAT_LONG));
  TEST("the directory opens for a LONG listing", err == FN_ERR_OK);

  bw = 0; conn = 0; nerr = 0;
  network_status(TNFS_BIG_DIR, &bw, &conn, &nerr);
  printf("  first STATUS: %u bytes waiting\n", bw);
  TEST("the first STATUS reports more than 512 bytes waiting", bw > 512);

  for (;;) {
    r = network_read(TNFS_BIG_DIR, (uint8_t *)chunk, sizeof(chunk));
    if (r <= 0)
      break;
    total += r;
    for (i = 0; i < r; i++) {
      if (DIR_IS_TERMINATOR((uint8_t)chunk[i])) {
        if (col)
          entries++;
        col = 0;
        continue;
      }
      if (!col)
        first = chunk[i];
      else if (col == 1 && first == 'Z' && chunk[i] == '/')
        saw_z = true;
      col++;
    }
  }
  network_close(TNFS_BIG_DIR);

  printf("  %u bytes, %u entries\n", total, entries);
  TEST("the listing is longer than 512 bytes", total > 512);
  TEST("the listing reaches Z/", saw_z);
#endif

  END_OF_TEST();
}

void test_fs_read_files(void)
{
  SECTION("network_fs: read back and overwrite");

#ifdef FN_BROKEN_network_fs
  SKIP(network_fs);
  END_OF_TEST();
#else
  fs_path("sub/alpha.txt");
  TEST("alpha.txt reads back", fs_get(g.fs.path) > 0);
  TEST("alpha.txt contents match", strcmp(g.fs.data, "alpha") == 0);

  fs_path("sub/beta.txt");
  TEST("beta.txt is overwritten", fs_put(g.fs.path, "beta-2") == FN_ERR_OK);
  TEST("beta.txt reads back", fs_get(g.fs.path) > 0);
  TEST("beta.txt has the new contents", strcmp(g.fs.data, "beta-2") == 0);

  END_OF_TEST();
#endif
}

void test_fs_rename_delete(void)
{
  uint8_t err;

  SECTION("network_fs: rename and delete");

#ifdef FN_BROKEN_network_fs
  SKIP(network_fs);
  END_OF_TEST();
#else
  /* rename takes both names in one devicespec, separated by a comma. */
  fs_path("sub/gamma.txt,renamed.txt");
  err = network_fs_rename(g.fs.path);
  TEST("network_fs_rename returns FN_ERR_OK", err == FN_ERR_OK);

  fs_path("sub/renamed.txt");
  TEST("renamed.txt reads back", fs_get(g.fs.path) > 0);
  TEST("renamed.txt keeps the original contents", strcmp(g.fs.data, "gamma") == 0);

  fs_path("sub/delta.txt");
  err = network_fs_delete(g.fs.path);
  TEST("network_fs_delete returns FN_ERR_OK", err == FN_ERR_OK);

  fs_path("sub/");
  printf("  contents of sub/\n");
  TEST("listing is not empty", fs_list(g.fs.path, OPEN_MODE_DIRECTORY, DIR_FORMAT_RAW) > 0);
  TEST("alpha.txt is still listed", fs_listed("alpha.txt"));
  TEST("renamed.txt is listed", fs_listed("renamed.txt"));
  TEST("gamma.txt is gone", !fs_listed("gamma.txt"));
  TEST("delta.txt is gone", !fs_listed("delta.txt"));

  END_OF_TEST();
#endif
}

void test_fs_dir_lifecycle(void)
{
  uint8_t err;

  SECTION("network_fs: mkdir and rmdir");

#ifdef FN_BROKEN_network_fs
  SKIP(network_fs);
  END_OF_TEST();
#else
  fs_path("doomed/");
  err = network_fs_mkdir(g.fs.path);
  TEST("mkdir of a throwaway directory returns FN_ERR_OK", err == FN_ERR_OK);

  err = fs_open_result(g.fs.path,
                       network_open(g.fs.path, OPEN_MODE_DIRECTORY, DIR_FORMAT_RAW));
  TEST("throwaway directory opens for listing", err == FN_ERR_OK);
  network_close(g.fs.path);

  err = network_fs_rmdir(g.fs.path);
  TEST("network_fs_rmdir returns FN_ERR_OK", err == FN_ERR_OK);

  err = fs_open_result(g.fs.path,
                       network_open(g.fs.path, OPEN_MODE_DIRECTORY, DIR_FORMAT_RAW));
  TEST("removed directory no longer opens", err != FN_ERR_OK);
  network_close(g.fs.path);

  END_OF_TEST();
#endif
}

#ifndef FN_BROKEN_network_fs
#define SD_FILTER_DIR "/fnfilter/"
#define SD_FILTER_URL "N1:SD://" SD_FILTER_DIR
#define SD_FILTER_ONE SD_FILTER_URL "one.txt"
#define SD_FILTER_TWO SD_FILTER_URL "two.txt"

/* Which of the two names the open directory lists, as bits 1 and 2. Closes it. */
static uint8_t fs_sd_listed(const char *one, const char *two)
{
  uint8_t seen = 0;
  uint8_t scan;

  for (scan = 0; scan < 32; scan++) {
    /* Without the details flag an entry is just the name. */
    memset(g.fs.listing, 0, 49);
    if (!fuji_read_directory(49, 0, g.fs.listing)
        || (uint8_t)g.fs.listing[0] == 0x7F)
      break;
    if (strcmp(g.fs.listing, one) == 0)
      seen |= 1;
    if (strcmp(g.fs.listing, two) == 0)
      seen |= 2;
  }
  fuji_close_directory();
  return seen;
}

static uint8_t fs_sd_write(const char *devicespec, const char *data)
{
  uint8_t err;

  err = network_open(devicespec, OPEN_MODE_WRITE, OPEN_TRANS_NONE);
  if (err != FN_ERR_OK)
    return err;
  err = network_write(devicespec, data, strlen(data));
  network_close(devicespec);
  return err;
}

static void fs_sd_filter_cleanup(void)
{
  network_fs_delete(SD_FILTER_ONE);
  network_fs_delete(SD_FILTER_TWO);
  network_fs_rmdir(SD_FILTER_URL);
}

/* The SD card is normally host slot 1; find it if it has moved. */
static uint8_t fs_sd_host(void)
{
  /* Not g.hslots: it overlays g.fs.base, which later sections still need. */
  static HostSlot hosts[MAX_HOSTS];
  uint8_t i;

  if (fuji_get_host_slots(&hosts[0], MAX_HOSTS)) {
    for (i = 0; i < MAX_HOSTS; i++) {
      if (strcmp((char *)hosts[i], "SD") == 0)
        return i;
    }
  }
  return 0;
}
#endif /* ! FN_BROKEN_network_fs */

void test_fs_directory_filter(void)
{
#ifndef FN_BROKEN_network_fs
  uint8_t sd_idx;
  uint8_t seen;
  bool ok;
#endif

  SECTION("fuji_open_directory_filter on the SD host");

#if defined(FN_BROKEN_network_fs) || defined(FN_BROKEN_fuji_open_directory_filter)
  SKIP(fuji_open_directory_filter);
  END_OF_TEST();
#else
  sd_idx = fs_sd_host();
  ok = fuji_mount_host_slot(sd_idx);
  TEST("SD host slot mounts", ok);

  fs_sd_filter_cleanup();
  TEST("fnfilter directory is created", network_fs_mkdir(SD_FILTER_URL) == FN_ERR_OK);
  TEST("one.txt is created", fs_sd_write(SD_FILTER_ONE, "one") == FN_ERR_OK);
  TEST("two.txt is created", fs_sd_write(SD_FILTER_TWO, "two") == FN_ERR_OK);

  ok = fuji_open_directory_filter(sd_idx, SD_FILTER_DIR, NULL);
  TEST("open with a NULL filter succeeds", ok);
  seen = ok ? fs_sd_listed("one.txt", "two.txt") : 0;
  TEST("a NULL filter lists both files", seen == 3);

  /* Stale bytes after the path must not be sent as the filter (#97). */
  memset(g.fs.path, 'X', sizeof(g.fs.path));
  strcpy(g.fs.path, SD_FILTER_DIR);
  ok = fuji_open_directory_filter(sd_idx, g.fs.path, "");
  TEST("open with an empty filter over a dirty buffer succeeds", ok);
  seen = ok ? fs_sd_listed("one.txt", "two.txt") : 0;
  TEST("an empty filter over a dirty buffer lists both files", seen == 3);

  ok = fuji_open_directory_filter(sd_idx, SD_FILTER_DIR, "one.*");
  TEST("open with the filter one.* succeeds", ok);
  seen = ok ? fs_sd_listed("one.txt", "two.txt") : 0;
  TEST("the filter one.* lists one.txt only", seen == 1);

  fs_sd_filter_cleanup();
  memset(g.fs.path, 0, sizeof(g.fs.path));
  strcpy(g.fs.path, SD_FILTER_DIR);
  ok = fuji_open_directory(sd_idx, g.fs.path);
  TEST("fnfilter directory is removed", !ok);
  fuji_close_directory();

  END_OF_TEST();
#endif
}

#ifndef FN_BROKEN_network_fs
#define SD_HASH_DIR  "/fnhash/"
#define SD_HASH_URL  "N1:SD://" SD_HASH_DIR
#define SD_HASH_FILE SD_HASH_URL "file#1.txt"
#define SD_HASH_ESC  SD_HASH_URL "file%231.txt"
#define SD_HASH_DATA "hash"

static void fs_sd_hash_cleanup(void)
{
  network_fs_delete(SD_HASH_FILE);
  /* Firmware that cuts the path at '#' writes this instead */
  network_fs_delete(SD_HASH_URL "file");
  network_fs_rmdir(SD_HASH_URL);
}
#endif /* ! FN_BROKEN_network_fs */

void test_fs_sd_hash_name(void)
{
#ifndef FN_BROKEN_network_fs
  uint8_t sd_idx;
  uint8_t seen;
  bool ok;
#endif

  SECTION("'#' in an SD filename");

#if defined(FN_BROKEN_network_fs) || defined(FN_BROKEN_fuji_open_directory_filter)
  SKIP(fuji_open_directory_filter);
  END_OF_TEST();
#else
  sd_idx = fs_sd_host();
  ok = fuji_mount_host_slot(sd_idx);
  TEST("SD host slot mounts", ok);

  fs_sd_hash_cleanup();
  TEST("fnhash directory is created", network_fs_mkdir(SD_HASH_URL) == FN_ERR_OK);
  TEST("file#1.txt is written", fs_sd_write(SD_HASH_FILE, SD_HASH_DATA) == FN_ERR_OK);
  TEST("file#1.txt reads back",
       fs_get(SD_HASH_FILE) == (int16_t) (sizeof(SD_HASH_DATA) - 1)
       && strcmp(g.fs.data, SD_HASH_DATA) == 0);

  ok = fuji_open_directory_filter(sd_idx, SD_HASH_DIR, NULL);
  seen = ok ? fs_sd_listed("file#1.txt", "file") : 0;
  TEST("the directory lists file#1.txt, not file", seen == 1);

  TEST("file%231.txt opens the same file",
       fs_get(SD_HASH_ESC) == (int16_t) (sizeof(SD_HASH_DATA) - 1)
       && strcmp(g.fs.data, SD_HASH_DATA) == 0);

  TEST("file#1.txt is deleted", network_fs_delete(SD_HASH_FILE) == FN_ERR_OK);
  ok = fuji_open_directory_filter(sd_idx, SD_HASH_DIR, NULL);
  seen = ok ? fs_sd_listed("file#1.txt", "file") : 3;
  TEST("file#1.txt is gone", seen == 0);

  fs_sd_hash_cleanup();

  END_OF_TEST();
#endif
}

void test_fs_lock_unlock(void)
{
  uint8_t err;

  SECTION("network_fs: lock and unlock");

#ifdef FN_BROKEN_network_fs
  SKIP(network_fs);
  END_OF_TEST();
#else
  /* HTTP overrides neither, so the base class accepts both and does
   * nothing. These record that, rather than that the file is protected. */
  fs_path("sub/alpha.txt");
  err = network_fs_lock(g.fs.path);
  TEST("network_fs_lock is accepted", err == FN_ERR_OK);
  TEST("a locked file can still be written", fs_put(g.fs.path, "alpha") == FN_ERR_OK);

  err = network_fs_unlock(g.fs.path);
  TEST("network_fs_unlock is accepted", err == FN_ERR_OK);

  END_OF_TEST();
#endif
}

#if defined(TNFS_RW_ROOT) && !defined(FN_BROKEN_network_fs)
#define TNFS_LOCK_FILE TNFS_RW_ROOT "fnlocktest.txt"

/* Open the file for writing and close it again, without writing. */
static uint8_t fs_tnfs_open_write(void)
{
  uint8_t err;

  err = fs_open_result(TNFS_LOCK_FILE,
                       network_open(TNFS_LOCK_FILE, OPEN_MODE_WRITE, OPEN_TRANS_NONE));
  network_close(TNFS_LOCK_FILE);
  return err;
}
#endif /* TNFS_RW_ROOT && ! FN_BROKEN_network_fs */

void test_fs_tnfs_lock_unlock(void)
{
#if defined(TNFS_RW_ROOT) && !defined(FN_BROKEN_network_fs)
  uint8_t err;
#endif

  SECTION("network_fs: TNFS lock and unlock");

#ifdef FN_BROKEN_network_fs
  SKIP(network_fs);
  END_OF_TEST();
#elif !defined(TNFS_RW_ROOT)
  /* Needs a TNFS server the test can write to, running a tnfsd that
   * answers CHMOD (FujiNetWIFI/tnfsd#34). */
  printf("SKIP  no writable TNFS server (TNFS_RW_ROOT)\n");
  tests_skipped++;
  END_OF_TEST();
#else
  TEST("the test file is created", fs_tnfs_open_write() == FN_ERR_OK);

  err = network_fs_lock(TNFS_LOCK_FILE);
  TEST("network_fs_lock succeeds", err == FN_ERR_OK);
  TEST("a locked file can't be opened for writing", fs_tnfs_open_write() != FN_ERR_OK);

  err = network_fs_unlock(TNFS_LOCK_FILE);
  TEST("network_fs_unlock succeeds", err == FN_ERR_OK);
  TEST("an unlocked file can be opened for writing", fs_tnfs_open_write() == FN_ERR_OK);

  err = network_fs_delete(TNFS_LOCK_FILE);
  TEST("the test file is deleted", err == FN_ERR_OK);

  END_OF_TEST();
#endif
}

void test_fs_ftp_listing(void)
{
  int16_t count;
  uint8_t err;

  SECTION("FTP directory listing");

#ifdef FN_BROKEN_network_fs
  SKIP(network_fs);
  END_OF_TEST();
#else
  printf("  contents of %s\n", FTP_HOST);
  count = fs_list(FTP_ROOT, OPEN_MODE_DIRECTORY, DIR_FORMAT_RAW);

  TEST("FTP directory listing is not empty", count > 0);

  /* welcome.msg sorts last, and the last entry was what the old
   * `dirBuffer.eof() ? UNSPECIFIED : NONE` threw away. */
  TEST("the last entry is not dropped", fs_listed("welcome.msg"));
  TEST("a plain file is listed", fs_listed("robots.txt"));

  TEST("a symlink keeps its own name", fs_listed("breakpoint/"));

  TEST("no entry is named ???", !fs_listed("???"));

  /* Without this the checks above would pass even if the open succeeded
   * unconditionally. */
  err = fs_open_result(FTP_MISSING_DIR,
                       network_open(FTP_MISSING_DIR, OPEN_MODE_DIRECTORY, DIR_FORMAT_RAW));
  TEST("an FTP directory that does not exist fails to open", err != FN_ERR_OK);
  network_close(FTP_MISSING_DIR);

  END_OF_TEST();
#endif
}

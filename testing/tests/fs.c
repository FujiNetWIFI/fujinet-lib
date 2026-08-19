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
static int16_t fs_list(const char *devicespec)
{
  int16_t r;
  int16_t count = 0;
  uint16_t i;
  uint8_t err;

  err = fs_open_result(devicespec,
                       network_open(devicespec, OPEN_MODE_HTTP_PROPFIND, DIR_FORMAT_RAW));
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
#ifdef FN_BROKEN_network_fs
  SECTION("network_fs_mkdir");
  SKIP(network_fs);
  END_OF_TEST();
#else
  char guid[MAX_GUID_LEN];
  uint8_t err;
  bool ok;

  SECTION("network_fs_mkdir");

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

#ifdef FN_BROKEN_network_fs_mkdir
  SKIP(network_fs_mkdir);
#else
  err = network_fs_mkdir(g.fs.base);
  TEST("network_fs_mkdir returns FN_ERR_OK", err == FN_ERR_OK);
#endif

  /* A WebDAV directory open only succeeds on a collection that is really
   * there, so this is what proves the mkdir took effect. */
  err = fs_open_result(g.fs.base,
                       network_open(g.fs.base, OPEN_MODE_HTTP_PROPFIND, DIR_FORMAT_RAW));
  TEST("created directory opens for listing", err == FN_ERR_OK);
  network_close(g.fs.base);

  /* Without this the check above would pass even if the open succeeded
   * unconditionally. */
  memset(g.fs.path, 0, sizeof(g.fs.path));
  strcpy(g.fs.path, WEBDAV_ROOT);
  strcat(g.fs.path, "fnfstest-never-created/");
  err = fs_open_result(g.fs.path,
                       network_open(g.fs.path, OPEN_MODE_HTTP_PROPFIND, DIR_FORMAT_RAW));
  TEST("directory that was never created does not open", err != FN_ERR_OK);
  network_close(g.fs.path);

  END_OF_TEST();
#endif
}

void test_fs_create_files(void)
{
#ifdef FN_BROKEN_network_fs
  SECTION("network_fs: subdirectory and files");
  SKIP(network_fs);
  END_OF_TEST();
#else
  int16_t count;
  uint8_t err;

  SECTION("network_fs: subdirectory and files");

  fs_path("sub/");
#ifdef FN_BROKEN_network_fs_mkdir
  SKIP(network_fs_mkdir);
#else
  err = network_fs_mkdir(g.fs.path);
  TEST("mkdir of a second level directory returns FN_ERR_OK", err == FN_ERR_OK);
#endif

  err = fs_open_result(g.fs.path,
                       network_open(g.fs.path, OPEN_MODE_HTTP_PROPFIND, DIR_FORMAT_RAW));
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
  count = fs_list(g.fs.path);
  TEST("directory listing is not empty", count > 0);
  TEST("alpha.txt is listed", fs_listed("alpha.txt"));
  TEST("beta.txt is listed", fs_listed("beta.txt"));
  TEST("gamma.txt is listed", fs_listed("gamma.txt"));
  TEST("delta.txt is listed", fs_listed("delta.txt"));

  END_OF_TEST();
#endif
}

void test_fs_read_files(void)
{
#ifdef FN_BROKEN_network_fs
  SECTION("network_fs: read back and overwrite");
  SKIP(network_fs);
  END_OF_TEST();
#else
  SECTION("network_fs: read back and overwrite");

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
#ifdef FN_BROKEN_network_fs
  SECTION("network_fs: rename and delete");
  SKIP(network_fs);
  END_OF_TEST();
#else
  uint8_t err;

  SECTION("network_fs: rename and delete");

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
  TEST("listing is not empty", fs_list(g.fs.path) > 0);
  TEST("alpha.txt is still listed", fs_listed("alpha.txt"));
  TEST("renamed.txt is listed", fs_listed("renamed.txt"));
  TEST("gamma.txt is gone", !fs_listed("gamma.txt"));
  TEST("delta.txt is gone", !fs_listed("delta.txt"));

  END_OF_TEST();
#endif
}

void test_fs_dir_lifecycle(void)
{
#ifdef FN_BROKEN_network_fs
  SECTION("network_fs: mkdir and rmdir");
  SKIP(network_fs);
  END_OF_TEST();
#else
  uint8_t err;

  SECTION("network_fs: mkdir and rmdir");

  fs_path("doomed/");
  err = network_fs_mkdir(g.fs.path);
  TEST("mkdir of a throwaway directory returns FN_ERR_OK", err == FN_ERR_OK);

  err = fs_open_result(g.fs.path,
                       network_open(g.fs.path, OPEN_MODE_HTTP_PROPFIND, DIR_FORMAT_RAW));
  TEST("throwaway directory opens for listing", err == FN_ERR_OK);
  network_close(g.fs.path);

  err = network_fs_rmdir(g.fs.path);
  TEST("network_fs_rmdir returns FN_ERR_OK", err == FN_ERR_OK);

  err = fs_open_result(g.fs.path,
                       network_open(g.fs.path, OPEN_MODE_HTTP_PROPFIND, DIR_FORMAT_RAW));
  TEST("removed directory no longer opens", err != FN_ERR_OK);
  network_close(g.fs.path);

  END_OF_TEST();
#endif
}

void test_fs_lock_unlock(void)
{
#ifdef FN_BROKEN_network_fs
  SECTION("network_fs: lock and unlock");
  SKIP(network_fs);
  END_OF_TEST();
#else
  uint8_t err;

  SECTION("network_fs: lock and unlock");

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

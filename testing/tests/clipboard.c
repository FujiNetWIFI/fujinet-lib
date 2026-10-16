/**
 * @file clipboard.c
 * @brief CLIPBOARD: protocol: snippets, history, append, and STATUS saying
 *        why a write or read was refused (fujinet-firmware #1819).
 *
 * These replace whatever is on the FujiNet's clipboard and its history.
 */

#include "broken.h"

#include "harness.h"
#include "globals.h"
#include <fujinet-network.h>

#ifndef _CMOC_VERSION_
#include <stdio.h>
#include <string.h>
#endif /* _CMOC_VERSION_ */

#define CLIP     "N1:CLIPBOARD:///"
#define CLIP0    "N1:CLIPBOARD:///0"
#define CLIP1    "N1:CLIPBOARD:///1"
#define CLIP2    "N1:CLIPBOARD:///2"
#define CLIP10   "N1:CLIPBOARD:///10"
#define CLIPX    "N1:CLIPBOARD:///x"
#define CLIP_BLOCK  512
#define CLIP_BLOCKS 128

static const char snip_a[] = "FujiNet clipboard A";
static const char snip_b[] = "FujiNet clipboard B";
static const char snip_c[] = "FujiNet clipboard C";
static const char snip_tail[] = " plus tail";
static const char snip_c_tail[] = "FujiNet clipboard C plus tail";

static uint8_t clip_put(const char *s, uint8_t mode)
{
  if (network_open(CLIP, mode, OPEN_TRANS_NONE) != FN_ERR_OK)
    return 0;
  if (*s && network_write(CLIP, s, strlen(s)) != FN_ERR_OK) {
    network_close(CLIP);
    return 0;
  }
  return network_close(CLIP) == FN_ERR_OK;
}

static int16_t clip_get(const char *spec)
{
  int16_t r;

  memset(g.net, 0, sizeof(g.net));
  if (network_open(spec, OPEN_MODE_READ, OPEN_TRANS_NONE) != FN_ERR_OK)
    return -1;
  r = network_read(spec, g.net, sizeof(g.net) - 1);
  network_close(spec);
  return r;
}

static uint8_t clip_is(const char *spec, const char *want)
{
  int16_t r = clip_get(spec);

  return r == (int16_t) strlen(want) && memcmp(g.net, want, r) == 0;
}

static uint8_t status_error(const char *spec)
{
  uint16_t bw;
  uint8_t conn, nerr = 0;

  network_status(spec, &bw, &conn, &nerr);
  printf("  net_error=%u\n", nerr);
  return nerr;
}

void test_clipboard(void)
{
  uint16_t bw;
  uint8_t conn, nerr, err;
  int16_t r;

  SECTION("clipboard snippets");

  TEST("write a snippet", clip_put(snip_a, OPEN_MODE_WRITE));
  TEST("/// reads it back", clip_is(CLIP, snip_a));
  TEST("///0 is the same snippet", clip_is(CLIP0, snip_a));

  err = network_open(CLIP, OPEN_MODE_READ, OPEN_TRANS_NONE);
  bw = 0; nerr = 0;
  network_status(CLIP, &bw, &conn, &nerr);
  TEST("status before reading shows the whole snippet waiting",
       err == FN_ERR_OK && bw == sizeof(snip_a) - 1 && nerr == NETWORK_SUCCESS);
  r = network_read(CLIP, g.net, sizeof(snip_a) - 1);
  bw = 1; nerr = 0;
  network_status(CLIP, &bw, &conn, &nerr);
  TEST("status after reading it all says end of file",
       r == sizeof(snip_a) - 1 && bw == 0 && nerr == NETWORK_ERROR_END_OF_FILE);
  network_close(CLIP);

  TEST("write a second snippet", clip_put(snip_b, OPEN_MODE_WRITE));
  TEST("write a third snippet", clip_put(snip_c, OPEN_MODE_WRITE));
  TEST("/// is the newest", clip_is(CLIP, snip_c));
  TEST("///1 is the one before", clip_is(CLIP1, snip_b));
  TEST("///2 is the first", clip_is(CLIP2, snip_a));

  TEST("append to the clipboard", clip_put(snip_tail, OPEN_MODE_APPEND));
  TEST("/// holds the snippet and its tail", clip_is(CLIP, snip_c_tail));

  END_OF_TEST();
}

void test_clipboard_refusals(void)
{
  uint8_t err;
  size_t r;

  SECTION("clipboard refusals and STATUS");

  err = network_open(CLIP1, OPEN_MODE_WRITE, OPEN_TRANS_NONE);
  TEST("write open on an older snippet fails", err != FN_ERR_OK);
  TEST("status says access denied", status_error(CLIP1) == NETWORK_ERROR_ACCESS_DENIED);
  network_close(CLIP1);

  err = network_open(CLIP, OPEN_MODE_READ, OPEN_TRANS_NONE);
  TEST("open the clipboard to read", err == FN_ERR_OK);
  err = network_write(CLIP, "x", 1);
  TEST("write on a read channel fails", err != FN_ERR_OK);
  TEST("status says read only", status_error(CLIP) == NETWORK_ERROR_READ_ONLY);
  network_close(CLIP);

  /* network_read() would wait for data a write channel never has */
  err = network_open(CLIP, OPEN_MODE_WRITE, OPEN_TRANS_NONE);
  TEST("open the clipboard to write", err == FN_ERR_OK);
  r = network_bus_read(FUJI_DEVICEID_NETWORK + network_unit(CLIP) - 1, g.net, 1);
  TEST("read on a write channel returns nothing", r == 0);
  TEST("status says write only", status_error(CLIP) == NETWORK_ERROR_WRITE_ONLY);
  network_close(CLIP);
  TEST("closing an empty write leaves the clipboard empty", clip_get(CLIP) == 0);

  err = network_open(CLIP10, OPEN_MODE_READ, OPEN_TRANS_NONE);
  TEST("snippet index 10 fails to open", err != FN_ERR_OK);
  TEST("status says invalid devicespec", status_error(CLIP10) == NETWORK_ERROR_INVALID_DEVICESPEC);
  network_close(CLIP10);

  err = network_open(CLIPX, OPEN_MODE_READ, OPEN_TRANS_NONE);
  TEST("a non-numeric index fails to open", err != FN_ERR_OK);
  TEST("status says invalid devicespec", status_error(CLIPX) == NETWORK_ERROR_INVALID_DEVICESPEC);
  network_close(CLIPX);

  err = network_open(CLIP, OPEN_MODE_DIRECTORY, OPEN_TRANS_NONE);
  TEST("a directory open fails", err != FN_ERR_OK);
  TEST("status says invalid command", status_error(CLIP) == NETWORK_ERROR_INVALID_COMMAND);
  network_close(CLIP);

  END_OF_TEST();
}

void test_clipboard_limit(void)
{
  uint8_t err;
  uint16_t i, blocks;

  SECTION("clipboard 64 KB limit");

  for (i = 0; i < CLIP_BLOCK; i++)
    g.net[i] = (uint8_t) ('a' + i % 26);

  err = network_open(CLIP, OPEN_MODE_WRITE, OPEN_TRANS_NONE);
  TEST("open the clipboard to write", err == FN_ERR_OK);
  for (blocks = 0; blocks < CLIP_BLOCKS; blocks++)
    if (network_write(CLIP, g.net, CLIP_BLOCK) != FN_ERR_OK)
      break;
  TEST("64 KB of writes are accepted", blocks == CLIP_BLOCKS);
  err = network_write(CLIP, g.net, 1);
  TEST("one more byte is refused", err != FN_ERR_OK);
  TEST("status says no space", status_error(CLIP) == NETWORK_ERROR_NO_SPACE_ON_DEVICE);
  network_close(CLIP);

  clip_get(CLIP);
  for (i = 0; i < CLIP_BLOCK - 1 && g.net[i] == (uint8_t) ('a' + i % 26); i++)
    ;
  TEST("the first 64 KB were kept", i == CLIP_BLOCK - 1);

  TEST("empty the clipboard", clip_put("", OPEN_MODE_WRITE));

  END_OF_TEST();
}

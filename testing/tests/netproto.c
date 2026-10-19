/**
 * @file netproto.c
 * @brief Reading every protocol to EOF the same way: STATUS, READ what it
 *        reports, until nothing is waiting and STATUS reports EOF. A new
 *        protocol gets this coverage by adding rows to proto_rows.
 */

#include "broken.h"

#include "harness.h"
#include "constants.h"
#include "globals.h"
#include "cmp_hex.h"
#include <fujinet-network.h>

#ifndef _CMOC_VERSION_
#include <stdio.h>
#include <string.h>
#endif /* _CMOC_VERSION_ */

/* STATUS polls with nothing waiting before giving up on a row */
#define IDLE_POLLS 1000
/* More than any row holds; a source that never ends stops here */
#define MAX_TOTAL 16384

#define FTP_WELCOME "N1:FTP://" FTP_HOST "/welcome.msg"
#define FTP_ROBOTS  "N1:FTP://" FTP_HOST "/robots.txt"
/* The server lists nothing in its root, so the WebDAV rows use a directory
 * of their own. */
#define DAV_DIR     WEBDAV_ROOT "fnproto/"
#define DAV_FILE    DAV_DIR "fnprototest.txt"
#define TNFS_FILE   TNFS_RW_ROOT "fnprototest.txt"
#define SD_DIR      "N1:SD:///fnproto/"
#define SD_FILE     SD_DIR "fnprototest.txt"
#define CLIP_URL    "N1:CLIPBOARD:///"
#define PUT_SIZE    300

/* One event inside the month CAL_ICAL_URL asks for */
static const char ICS_BODY[] =
  "BEGIN:VCALENDAR\r\nVERSION:2.0\r\nPRODID:-//FujiNet//fujinet-lib tests//EN\r\n"
  "BEGIN:VEVENT\r\nUID:fnlibtest-1@fujinet.online\r\nDTSTAMP:20260901T000000Z\r\n"
  "DTSTART:20260915T150000Z\r\nDTEND:20260915T160000Z\r\n"
  "SUMMARY:" CAL_EVENT_SUMMARY "\r\nEND:VEVENT\r\nEND:VCALENDAR\r\n";

enum {
  SETUP_NONE,
  SETUP_DIR,   /* made before the rows, removed after */
  SETUP_FILE,  /* PUT_SIZE patterned bytes written before, deleted after */
  SETUP_CLIP,  /* written before; the clipboard has nothing to delete */
  SETUP_ICS,   /* ICS_BODY PUT to CAL_ICS_PUT_URL before, deleted after */
};

typedef struct {
  const char *name;
  const char *url;
  uint8_t mode;
  uint16_t size;     /* 0: unknown, any data will do */
  uint8_t pattern;   /* bytes follow RANGE_BYTE() */
  uint8_t setup;
  uint8_t put_mode;  /* how SETUP_FILE and SETUP_CLIP open it */
} proto_row;

/* Every protocol the tests can reach has a row here, a file row and, where
 * the protocol has directories, a directory row; a new protocol adds its
 * rows. Setup runs top to bottom and cleanup bottom to top, so a directory
 * comes before the file in it. */
static const proto_row proto_rows[] = {
  { "HTTP file",      NET_RANGE_URL, OPEN_MODE_READ,      1024,     1, SETUP_NONE, 0 },
  { "FTP file",       FTP_WELCOME,   OPEN_MODE_READ,      1329,     0, SETUP_NONE, 0 },
  { "FTP small file", FTP_ROBOTS,    OPEN_MODE_READ,      26,       0, SETUP_NONE, 0 },
  { "FTP dir",        FTP_ROOT,      OPEN_MODE_DIRECTORY, 0,        0, SETUP_NONE, 0 },
  { "HTTP(S)/WebDAV dir", DAV_DIR,       OPEN_MODE_DIRECTORY, 0,        0, SETUP_DIR,  0 },
  { "WebDAV file",    DAV_FILE,      OPEN_MODE_READ,      PUT_SIZE, 1, SETUP_FILE, OPEN_MODE_HTTP_PUT },
#ifdef TNFS_RW_ROOT
  { "TNFS dir",       TNFS_RW_ROOT,  OPEN_MODE_DIRECTORY, 0,        0, SETUP_NONE, 0 },
  { "TNFS file",      TNFS_FILE,     OPEN_MODE_READ,      PUT_SIZE, 1, SETUP_FILE, OPEN_MODE_WRITE },
#endif /* TNFS_RW_ROOT */
  { "SD dir",         SD_DIR,        OPEN_MODE_DIRECTORY, 0,        0, SETUP_DIR,  0 },
  { "SD file",        SD_FILE,       OPEN_MODE_READ,      PUT_SIZE, 1, SETUP_FILE, OPEN_MODE_WRITE },
  { "CLIPBOARD",      CLIP_URL,      OPEN_MODE_READ,      PUT_SIZE, 1, SETUP_CLIP, OPEN_MODE_WRITE },
  { "ICAL",           CAL_ICAL_URL,  OPEN_MODE_READ,      0,        0, SETUP_ICS,  0 },
};

#define ROW_COUNT (sizeof(proto_rows) / sizeof(proto_rows[0]))

static char tname[64];

static const char *row_test(const proto_row *row, const char *what)
{
  strcpy(tname, row->name);
  strcat(tname, ": ");
  strcat(tname, what);
  return tname;
}

static void read_to_eof(const proto_row *row)
{
  uint8_t err, conn, nerr, eof, pattern_ok, count_ok;
  uint16_t bw, polls, total, n, i;
  int16_t r;

  err = network_open(row->url, row->mode, OPEN_TRANS_NONE);
  TEST(row_test(row, "opens"), err == FN_ERR_OK);

  total = 0;
  eof = 0;
  pattern_ok = 1;
  count_ok = 1;
  for (polls = 0; polls < IDLE_POLLS && total < MAX_TOTAL; ) {
    bw = 0; conn = 0; nerr = 0;
    if (network_status(row->url, &bw, &conn, &nerr) != FN_ERR_OK)
      break;
    if (!bw) {
      if (!conn || nerr == NETWORK_ERROR_END_OF_FILE) {
        eof = 1;
        break;
      }
      polls++;
      continue;
    }
    n = bw > sizeof(g.net) ? sizeof(g.net) : bw;
    r = network_read(row->url, g.net, n);
    if (r != (int16_t) n) {
      count_ok = 0;
      break;
    }
    if (row->pattern)
      for (i = 0; i < n; i++)
        if (g.net[i] != RANGE_BYTE(total + i))
          pattern_ok = 0;
    total += n;
    polls = 0;
  }
  printf("  total=%u conn=%u net_error=%u\n", total, conn, nerr);

  TEST(row_test(row, "READ returns what STATUS reported"), count_ok);
  TEST(row_test(row, "reaches the end"), eof);
  TEST(row_test(row, "STATUS at the end says EOF"), nerr == NETWORK_ERROR_END_OF_FILE);
  if (row->size)
    TEST(row_test(row, "size matches"), total == row->size);
  else
    TEST(row_test(row, "returns data"), total > 0);
  if (row->pattern)
    TEST(row_test(row, "bytes match"), pattern_ok);

  r = network_read_nb(row->url, g.net, 16);
  TEST(row_test(row, "read past EOF returns no data"), r <= 0);

  err = network_close(row->url);
  TEST(row_test(row, "closes"), err == FN_ERR_OK);
}

static uint8_t put_file(const char *url, uint8_t mode)
{
  uint16_t i;

  for (i = 0; i < PUT_SIZE; i++)
    g.net[i] = RANGE_BYTE(i);
  if (network_open(url, mode, OPEN_TRANS_NONE) != FN_ERR_OK)
    return 0;
  if (network_write(url, g.net, PUT_SIZE) != FN_ERR_OK) {
    network_close(url);
    return 0;
  }
  return network_close(url) == FN_ERR_OK;
}

static uint8_t put_ics(void)
{
  if (network_open(CAL_ICS_PUT_URL, OPEN_MODE_HTTP_PUT, OPEN_TRANS_NONE) != FN_ERR_OK)
    return 0;
  if (network_write(CAL_ICS_PUT_URL, ICS_BODY, sizeof(ICS_BODY) - 1) != FN_ERR_OK) {
    network_close(CAL_ICS_PUT_URL);
    return 0;
  }
  return network_close(CAL_ICS_PUT_URL) == FN_ERR_OK;
}

static void row_setup(const proto_row *row)
{
  switch (row->setup) {
  case SETUP_DIR:
    /* Fails harmlessly if a stopped run left it behind */
    network_fs_mkdir(row->url);
    break;
  case SETUP_FILE:
  case SETUP_CLIP:
    TEST(row_test(row, "is written"), put_file(row->url, row->put_mode));
    break;
  case SETUP_ICS:
    TEST(row_test(row, "feed is written"), put_ics());
    break;
  }
}

static void row_cleanup(const proto_row *row)
{
  switch (row->setup) {
  case SETUP_DIR:
    TEST(row_test(row, "is removed"), network_fs_rmdir(row->url) == FN_ERR_OK);
    break;
  case SETUP_FILE:
    TEST(row_test(row, "is deleted"), network_fs_delete(row->url) == FN_ERR_OK);
    break;
  case SETUP_ICS:
    TEST(row_test(row, "feed is deleted"), network_fs_delete(CAL_ICS_PUT_URL) == FN_ERR_OK);
    break;
  }
}

void test_netproto_read_count(void)
{
  uint8_t err;
  int16_t r;
  uint16_t i;

  SECTION("network read honors requested count");

  err = network_open(NET_RANGE_URL, OPEN_MODE_READ, OPEN_TRANS_NONE);
  TEST("open range URL", err == FN_ERR_OK);
  memset(g.net, 0, sizeof(g.net));
  r = network_read(NET_RANGE_URL, g.net, 64);
  TEST("read returns exactly 64 bytes", r == 64);
  for (i = 0; i < 64 && g.net[i] == RANGE_BYTE(i); i++)
    ;
  TEST("64 bytes match pattern", i == 64);
  err = network_close(NET_RANGE_URL);
  TEST("close range URL", err == FN_ERR_OK);

  END_OF_TEST();
}

void test_netproto_read_to_eof(void)
{
  uint8_t i;

  SECTION("read every protocol to EOF");

  for (i = 0; i < ROW_COUNT; i++)
    row_setup(&proto_rows[i]);
  for (i = 0; i < ROW_COUNT; i++)
    read_to_eof(&proto_rows[i]);
  for (i = ROW_COUNT; i > 0; i--)
    row_cleanup(&proto_rows[i - 1]);

  END_OF_TEST();
}

/* TCP is a stream with no EOF to read to, so it gets its own check: a read
 * for part of what is waiting returns exactly that part. */
void test_netproto_read_partial(void)
{
  uint8_t err;
  int16_t r, w, w2;
  static const uint8_t msg[] = "PRT" ECHO_MSG BASIC_LINE_ENDING;

  SECTION("network_read_partial");

  err = network_open(NET_TCP_SPEC, OPEN_MODE_RW, OPEN_TRANS_LF);
  if (err != FN_ERR_OK)
    TEST("network_open (TCP RW) succeeds", err == FN_ERR_OK);

  w = strlen((const char *) msg);
  err = network_write(NET_TCP_SPEC, msg, w);
  if (err != FN_ERR_OK)
    TEST("network_write succeeds", err == FN_ERR_OK);

  memset(g.net, 0, sizeof(g.net));
  w2 = w / 2;
  r = network_read(NET_TCP_SPEC, g.net, w2);
  TEST("read partial correct length", r == w2);
  r = network_read(NET_TCP_SPEC, &g.net[w2], w - w2);
  TEST("read remaining correct length", r == w - w2);
  if (memcmp(g.net, msg, w) != 0)
    cmp_hex("orig", msg, w, "recv", g.net, r);
  TEST("all data read", memcmp(g.net, msg, w) == 0);

  network_close(NET_TCP_SPEC);

  END_OF_TEST();
}

#include "broken.h"

#include "harness.h"
#include "constants.h"
#include "globals.h"
#include <fujinet-network.h>

#ifndef _CMOC_VERSION_
#include <stdio.h>
#endif /* _CMOC_VERSION_ */

/* Small, stable public FTP file used to exercise the STATUS/READ boundary. */
#define FTP_FILE "N1:FTP://" FTP_HOST "/robots.txt"

void test_ftp_file_status_read(void)
{
  uint16_t bytes_waiting;
  int16_t read_count;
  uint8_t err;
  uint8_t conn, nerr, attempt;
  uint8_t saw_data, reached_eof;

  SECTION("FTP file STATUS then READ");

#if defined(FN_BROKEN_network_open) || defined(FN_BROKEN_network_status) \
  || defined(FN_BROKEN_network_read)
#ifdef FN_BROKEN_network_open
  SKIP(network_open);
#endif
#ifdef FN_BROKEN_network_status
  SKIP(network_status);
#endif
#ifdef FN_BROKEN_network_read
  SKIP(network_read);
#endif
#else
  saw_data = 0;
  reached_eof = 0;

  err = network_open(FTP_FILE, OPEN_MODE_HTTP_GET, OPEN_TRANS_NONE);
  TEST("FTP file opens", err == FN_ERR_OK);

  if (err == FN_ERR_OK) {
    /* This is deliberately the client pattern from PR #1690: ask STATUS
     * how many bytes are ready, then READ exactly that many bytes. */
    for (attempt = 0; attempt < 32 && !reached_eof; attempt++) {
      bytes_waiting = 0;
      conn = 0;
      nerr = 0;
      err = network_status(FTP_FILE, &bytes_waiting, &conn, &nerr);
      TEST("FTP STATUS succeeds", err == FN_ERR_OK);
      if (err != FN_ERR_OK)
        break;

      printf("  bytes_waiting=%u conn=%u net_error=%u\n",
             bytes_waiting, conn, nerr);
      if (bytes_waiting == 0) {
        if (!conn)
          reached_eof = 1;
        continue;
      }

      TEST("FTP STATUS fits the READ buffer", bytes_waiting <= sizeof(g.net));
      if (bytes_waiting > sizeof(g.net))
        break;

      read_count = network_read(FTP_FILE, g.net, bytes_waiting);
      TEST("FTP READ returns STATUS byte count",
           read_count == (int16_t)bytes_waiting);
      if (read_count <= 0)
        break;
      saw_data = 1;
    }

    network_close(FTP_FILE);
  }

  /* On firmware before PR #1690, STATUS reports zero here after it has
   * prefetched the file into receiveBuffer, so this assertion catches the
   * lost-file regression instead of merely checking that the socket opened. */
  TEST("FTP STATUS/READ returns file data", saw_data);
  TEST("FTP STATUS/READ reaches EOF", reached_eof);
#endif

  END_OF_TEST();
}

/* The server has results.txt. The upper-case 8.3 name is what a listing
 * shows, and only the listing in resolve() can match it (fujinet-firmware
 * #1720), so this fails on firmware whose FTP stat() never fails. */
#define FTP_CRUNCHED "N1:FTP://" FTP_HOST "/breakpoint/2003/info/RESULTS.TXT"

void test_ftp_crunched_name(void)
{
  uint16_t bytes_waiting;
  int16_t read_count;
  uint8_t err;
  uint8_t conn, nerr, attempt;

  SECTION("FTP open by a crunched 8.3 name");

#if defined(FN_BROKEN_network_open) || defined(FN_BROKEN_network_status) \
  || defined(FN_BROKEN_network_read)
#ifdef FN_BROKEN_network_open
  SKIP(network_open);
#endif
#ifdef FN_BROKEN_network_status
  SKIP(network_status);
#endif
#ifdef FN_BROKEN_network_read
  SKIP(network_read);
#endif
#else
  err = network_open(FTP_CRUNCHED, OPEN_MODE_READ, OPEN_TRANS_NONE);
  TEST("FTP opens results.txt as RESULTS.TXT", err == FN_ERR_OK);

  if (err == FN_ERR_OK) {
    bytes_waiting = 0;
    conn = 1;
    for (attempt = 0; attempt < 32 && !bytes_waiting && conn; attempt++) {
      err = network_status(FTP_CRUNCHED, &bytes_waiting, &conn, &nerr);
      if (err != FN_ERR_OK)
        break;
    }

    if (bytes_waiting > sizeof(g.net))
      bytes_waiting = sizeof(g.net);
    read_count = bytes_waiting ? network_read(FTP_CRUNCHED, g.net, bytes_waiting) : 0;
    TEST("FTP READ returns data from results.txt", read_count > 0);

    network_close(FTP_CRUNCHED);
  }
#endif

  END_OF_TEST();
}

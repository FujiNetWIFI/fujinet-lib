#include "broken.h"

#include "harness.h"
#include "constants.h"
#include "globals.h"
#include <fujinet-network.h>

#ifndef _CMOC_VERSION_
#include <stdio.h>
#endif /* _CMOC_VERSION_ */

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

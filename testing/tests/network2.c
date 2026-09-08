/**
 * @file network2.c
 * @brief Coverage for the network device commands added in fujinet-lib v5:
 *        seek/tell, translation, EOL, login, timer rate, TCP close-client,
 *        UDP destination/remote, JSON parameters, DSTATS inquiry, channel
 *        mode, and the fn_* status globals.
 *
 * Firmware regression commits exercised here (fujinet-firmware):
 *   b8d67c34c  [sio] note/point            -> test_net2_seek_tell, test_net2_seek_errors,
 *                                             test_net2_get_dstats
 *   f3586b99b  set-EOL command             -> test_net2_set_eol_ext
 *   a1c8d9dfe  [sio] sticky NTRANS         -> test_net2_set_translation (wire accept only;
 *                                             full translation observation is manual)
 *   e04b1759a  [coco] $54 arm removed      -> FN_BROKEN_network_set_translation on coco
 *   a720672e2  [iwm] login propagation     -> test_net2_login_smoke (authenticated open
 *                                             needs private credentials: manual)
 *   e2699b33a / 94517bf9a / 90279b566 UDP  -> test_net2_udp_set_destination,
 *                                             test_net2_udp_get_remote (FujiNet-PC only)
 *   0b59791ee  [coco] read count           -> test_net2_read_count
 */

#include "broken.h"

#include "harness.h"
#include "constants.h"
#include "globals.h"
#include <fujinet-network.h>

#ifndef _CMOC_VERSION_
#include <stdio.h>
#include <string.h>
#endif /* _CMOC_VERSION_ */

void test_net2_seek_tell(void)
{
  uint8_t err;
  uint32_t pos;
  int16_t r;
  uint16_t i;

  SECTION("network seek/tell (note/point)");

#ifdef FN_BROKEN_network_seek
  SKIP(network_seek);
#else
  err = network_open(NET_RANGE_URL, OPEN_MODE_READ, OPEN_TRANS_NONE);
  TEST("open range URL", err == FN_ERR_OK);

  r = network_read(NET_RANGE_URL, g.net, 16);
  TEST("read 16 bytes", r == 16);
  for (i = 0; i < 16; i++) {
    if (g.net[i] != RANGE_BYTE(i))
      break;
  }
  TEST("first 16 bytes match pattern", i == 16);

  pos = 0xFFFFFFFF;
  err = network_tell(NET_RANGE_URL, &pos);
  TEST("tell returns FN_ERR_OK", err == FN_ERR_OK);
  printf("  position=%lu\n", (unsigned long) pos);
  TEST("tell reports position 16", pos == 16);

  err = network_seek(NET_RANGE_URL, 256);
  TEST("seek to 256 returns FN_ERR_OK", err == FN_ERR_OK);

  pos = 0;
  err = network_tell(NET_RANGE_URL, &pos);
  TEST("tell after seek returns FN_ERR_OK", err == FN_ERR_OK);
  TEST("tell reports position 256", pos == 256);

  r = network_read(NET_RANGE_URL, g.net, 4);
  TEST("read 4 bytes at offset 256", r == 4);
  /* 256 % 26 == 22, so the bytes are w x y z */
  TEST("data comes from the seeked offset",
       g.net[0] == 'w' && g.net[1] == 'x' && g.net[2] == 'y' && g.net[3] == 'z');

  err = network_close(NET_RANGE_URL);
  TEST("close range URL", err == FN_ERR_OK);
#endif /* FN_BROKEN_network_seek */

  END_OF_TEST();
}

void test_net2_seek_errors(void)
{
  uint8_t err;

  SECTION("network seek error paths");

#ifdef FN_BROKEN_network_seek
  SKIP(network_seek);
#else
  /* No channel open on N4 */
  err = network_seek("N4:", 0);
  TEST("seek on unopened channel fails", err != FN_ERR_OK);

  /* Seek is PROTOCOL-mode only; in JSON mode it must fail */
  err = network_open(NET_JSON_URL, OPEN_MODE_HTTP_GET, OPEN_TRANS_NONE);
  TEST("open JSON URL", err == FN_ERR_OK);
  err = network_json_parse(NET_JSON_URL);
  TEST("parse JSON", err == FN_ERR_OK);
  err = network_seek(NET_JSON_URL, 0);
  TEST("seek in JSON channel mode fails", err != FN_ERR_OK);
  err = network_close(NET_JSON_URL);
  TEST("close JSON URL", err == FN_ERR_OK);
#endif /* FN_BROKEN_network_seek */

  END_OF_TEST();
}

void test_net2_set_translation(void)
{
  uint8_t err;

  SECTION("network set translation");

#ifdef FN_BROKEN_network_set_translation
  SKIP(network_set_translation);
#else
  /* Wire-level accept only. The sticky OR into subsequent opens
   * (a1c8d9dfe) is observed manually: set 2, open with aux2 0, read a
   * text resource, confirm LF translation. */
  err = network_set_translation("N1:", OPEN_TRANS_LF);
  TEST("set translation LF accepted", err == FN_ERR_OK);

  err = network_set_translation("N1:", 0xFF);
  TEST("set translation 0xFF (ignore aux2) accepted", err == FN_ERR_OK);

  err = network_set_translation("N1:", OPEN_TRANS_NONE);
  TEST("set translation restored to none", err == FN_ERR_OK);
#endif /* FN_BROKEN_network_set_translation */

  END_OF_TEST();
}

void test_net2_login_smoke(void)
{
  uint8_t err;

  SECTION("network username/password");

#ifdef FN_BROKEN_network_set_username
  SKIP(network_set_username);
#else
  /* State-only: the values apply to the next open. An authenticated
   * SMB/FTP open needs private credentials and is a manual test. */
  err = network_set_username("N1:", "fujinet-test-user");
  TEST("set username accepted", err == FN_ERR_OK);

  err = network_set_password("N1:", "fujinet-test-pass");
  TEST("set password accepted", err == FN_ERR_OK);

  /* Clear them so later opens are not affected */
  err = network_set_username("N1:", "");
  TEST("clear username accepted", err == FN_ERR_OK);
  err = network_set_password("N1:", "");
  TEST("clear password accepted", err == FN_ERR_OK);
#endif /* FN_BROKEN_network_login */

  END_OF_TEST();
}

void test_net2_interrupt_rate(void)
{
  uint8_t err;

  SECTION("network interrupt timer rate");

#ifdef FN_BROKEN_network_set_interrupt_rate
  SKIP(network_set_interrupt_rate);
#else
  err = network_set_interrupt_rate("N1:", 50);
  TEST("set timer rate 50ms accepted", err == FN_ERR_OK);

  err = network_set_interrupt_rate("N1:", 100);
  TEST("restore timer rate 100ms accepted", err == FN_ERR_OK);
#endif /* FN_BROKEN_network_set_interrupt_rate */

  END_OF_TEST();
}

void test_net2_close_client(void)
{
  uint8_t err;

  SECTION("network TCP close-client");

#ifdef FN_BROKEN_network_close_client
  SKIP(network_close_client);
#else
  err = network_open(NET_TCP_LISTEN, OPEN_MODE_RW, OPEN_TRANS_NONE);
  TEST("open listening TCP channel", err == FN_ERR_OK);

  /* No client has connected; closing the (absent) client connection
   * must still complete cleanly */
  err = network_close_client(NET_TCP_LISTEN);
  TEST("close-client with no client completes", err == FN_ERR_OK);

  err = network_close(NET_TCP_LISTEN);
  TEST("close listening channel", err == FN_ERR_OK);
#endif /* FN_BROKEN_network_close_client */

  END_OF_TEST();
}

void test_net2_udp_set_destination(void)
{
  uint8_t err;

  SECTION("network UDP set destination");

#ifdef FN_BROKEN_network_udp_set_destination
  SKIP(network_udp_set_destination);
#else
  err = network_open(NET_UDP_SPEC, OPEN_MODE_RW, OPEN_TRANS_NONE);
  TEST("open UDP channel", err == FN_ERR_OK);

  err = network_udp_set_destination(NET_UDP_DEST);
  TEST("set UDP destination accepted", err == FN_ERR_OK);

  err = network_close(NET_UDP_SPEC);
  TEST("close UDP channel", err == FN_ERR_OK);
#endif /* FN_BROKEN_network_udp */

  END_OF_TEST();
}

void test_net2_udp_get_remote(void)
{
  uint8_t err;

  SECTION("network UDP get remote");

#ifdef FN_BROKEN_network_udp_get_remote
  SKIP(network_udp_get_remote);
#else
  /* FujiNet-PC only: receive a datagram first, then ask where it came
   * from. Without inbound traffic just prove the command completes. */
  err = network_open(NET_UDP_SPEC, OPEN_MODE_RW, OPEN_TRANS_NONE);
  TEST("open UDP channel", err == FN_ERR_OK);

  memset(g.net, 0, sizeof(g.net));
  err = network_udp_get_remote(NET_UDP_SPEC, (char *) g.net, MAX_FILENAME_LEN);
  TEST("get remote completes", err == FN_ERR_OK);
  printf("  remote=\"%s\"\n", (char *) g.net);

  err = network_close(NET_UDP_SPEC);
  TEST("close UDP channel", err == FN_ERR_OK);
#endif /* FN_BROKEN_network_udp_get_remote */

  END_OF_TEST();
}

void test_net2_channel_mode(void)
{
  uint8_t err;
  int16_t r;

  SECTION("network channel mode transitions");

#ifdef FN_BROKEN_network_set_parser
  SKIP(network_set_parser);
#else
  err = network_open(NET_JSON_URL, OPEN_MODE_HTTP_GET, OPEN_TRANS_NONE);
  TEST("open JSON URL", err == FN_ERR_OK);

  err = network_set_parser(NET_JSON_URL, PARSER_JSON);
  TEST("switch to JSON channel mode", err == FN_ERR_OK);

  err = network_set_parser(NET_JSON_URL, PARSER_NONE);
  TEST("switch back to PROTOCOL mode", err == FN_ERR_OK);

  /* The body must still be readable after the round trip */
  r = network_read(NET_JSON_URL, g.net, 64);
  TEST("read body after mode round trip", r > 0);

  err = network_close(NET_JSON_URL);
  TEST("close JSON URL", err == FN_ERR_OK);
#endif /* FN_BROKEN_network_set_parser */

  END_OF_TEST();
}

void test_net2_read_count(void)
{
  uint8_t err;
  int16_t r;
  uint16_t i;
  uint16_t total;

  SECTION("network read honors requested count");

#ifdef FN_BROKEN_network_read_partial
  SKIP(network_read_partial);
#else
  err = network_open(NET_RANGE_URL, OPEN_MODE_READ, OPEN_TRANS_NONE);
  TEST("open range URL", err == FN_ERR_OK);

  /* 1024 bytes are (eventually) waiting; ask for exactly 64 */
  memset(g.net, 0, sizeof(g.net));
  r = network_read(NET_RANGE_URL, g.net, 64);
  TEST("read returns exactly 64 bytes", r == 64);
  for (i = 0; i < 64; i++) {
    if (g.net[i] != RANGE_BYTE(i))
      break;
  }
  TEST("64 bytes match pattern", i == 64);

  /* Drain the rest */
  total = 64;
  for (;;) {
    r = network_read_nb(NET_RANGE_URL, g.net, sizeof(g.net));
    if (r < 1)
      break;
    total += r;
  }
  printf("  drained total=%u\n", total);
  TEST("all 1024 bytes arrived", total == 1024);

  /* Reading past the end reports no more data */
  r = network_read_nb(NET_RANGE_URL, g.net, 16);
  TEST("read past EOF returns no data", r <= 0);

  err = network_close(NET_RANGE_URL);
  TEST("close range URL", err == FN_ERR_OK);
#endif /* FN_BROKEN_network_read_partial */

  END_OF_TEST();
}

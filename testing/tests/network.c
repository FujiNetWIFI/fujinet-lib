#include "broken.h"

#include "harness.h"
#include "constants.h"
#include "globals.h"
#include "cmp_hex.h"
#include "print_hex.h"
#include <fujinet-network.h>

#ifndef _CMOC_VERSION_
#include <stdio.h>
#include <string.h>
#endif /* _CMOC_VERSION_ */

//#define ALT_LINE_ENDING "\n#"
#define ALT_LINE_ENDING "\x0d\x0a"

/* Back-to-back STATUS polls; the full count only runs when no echo comes. */
#define ECHO_WAIT_RETRIES 500
/* By the time the echo is back, a leak onto the other unit would be too. */
#define ECHO_LEAK_POLLS 5

void mark_echo_message(const char *prefix, uint8_t msg[])
{
  size_t len = strlen(prefix);
  uint8_t *r = (uint8_t *) strchr((char *) msg, ' ');


  if (r - msg < len)
    len = r - msg;
  memcpy(msg, prefix, len);
  return;
}

void test_network_init(void)
{
  uint8_t err;

  SECTION("network_init");
#ifdef FN_BROKEN_network_init
  SKIP(network_init);
#else
  err = network_init();
  TEST("network_init returns FN_ERR_OK", err == FN_ERR_OK);
#endif

  END_OF_TEST();
}

void echo_check(const uint8_t *msg)
{
  uint8_t err;
  int16_t r, w;

#ifdef FN_BROKEN_network_write
  SKIP(network_write);
#else
  w = strlen((const char *) msg);
  err = network_write(NET_TCP_SPEC, msg, w);
  TEST("network_write succeeds", err == FN_ERR_OK);
#endif

#ifdef FN_BROKEN_network_read
  SKIP(network_read);
#else
  memset(g.net, 0, sizeof(g.net));
  r = network_read(NET_TCP_SPEC, g.net, w);
  TEST("network_read echoed data", r > 0);
  if (r > 0) {
    if (r > w + 1)
      r = w + 1;
    if (memcmp(g.net, msg, w) != 0)
      cmp_hex("orig", msg, w, "recv", g.net, r);
    TEST("Echo matches sent message", memcmp(g.net, msg, w) == 0);
  }
#endif

#ifdef FN_BROKEN_network_close
  SKIP(network_close);
#else
  err = network_close(NET_TCP_SPEC);
  TEST("network_close TCP succeeds", err == FN_ERR_OK);
#endif

  END_OF_TEST();
}

void test_network_write(void)
{
  uint8_t err;
  int16_t w;
  uint8_t msg[] = ECHO_MSG BASIC_LINE_ENDING;

  SECTION("network_write (raw TCP)");

#ifdef FN_BROKEN_network_open
  SKIP(network_open);
#else
  err = network_open(NET_TCP_SPEC, OPEN_MODE_RW, OPEN_TRANS_LF);
  TEST("network_open (TCP RW) succeeds", err == FN_ERR_OK);
#endif

  mark_echo_message("WRT", msg);
  echo_check(msg);

#ifdef FN_BROKEN_network_open_invalid
  SKIP(network_open_invalid);
#endif
  err = network_open("N1:TCP://BOGUS_HOST_NAME:1234", OPEN_MODE_RW, OPEN_TRANS_NONE);
  TEST("network_open on invalid host fails", err != FN_ERR_OK);

  w = strlen((const char *) msg);
  err = network_write(NET_TCP_SPEC, msg, w);
  TEST("network_write on closed connection fails", err != FN_ERR_OK);

  // FIXME - test that read on closed connection fails without crashing firmware

  END_OF_TEST();
}

void test_network_set_eol(void)
{
  uint8_t err;
  uint8_t msg[] = ECHO_MSG ALT_LINE_ENDING;

  SECTION("network_set_eol (raw TCP)");

#ifdef FN_BROKEN_network_open
  SKIP(network_open);
#else
  err = network_open(NET_TCP_SPEC, OPEN_MODE_RW, OPEN_TRANS_LF);
  TEST("network_open (TCP RW) succeeds", err == FN_ERR_OK);
#endif

#ifdef FN_BROKEN_network_set_eol
  SKIP(network_set_eol);
#else
  err = network_set_eol(NET_TCP_SPEC, ALT_LINE_ENDING);
  TEST("network_set_eol succeeds", err == FN_ERR_OK);
  print_hex("ALT EOL", (uint8_t *)ALT_LINE_ENDING, strlen(ALT_LINE_ENDING));

  mark_echo_message("EOL", msg);
  echo_check(msg);

#ifdef FN_BROKEN_network_set_eol
  SKIP(network_set_eol);
#else
  err = network_set_eol(NET_TCP_SPEC, "");
  TEST("network_set_eol reset succeeds", err == FN_ERR_OK);
#endif

#endif // FN_BROKEN_network_set_eol

  END_OF_TEST();
}

void test_network_unit(void)
{
  SECTION("network_unit helper");
#ifdef FN_BROKEN_network_unit
  SKIP(network_unit);
#else
  TEST("network_unit('N1:...') == 1", network_unit("N1:HTTPS://fujinet.online/") == 1);
  TEST("network_unit('N2:...') == 2", network_unit("N2:HTTPS://fujinet.online/") == 2);
  TEST("network_unit('N8:...') == 8", network_unit("N8:HTTPS://fujinet.online/") == 8);
#endif

  END_OF_TEST();
}

void test_network_error_path(void)
{
  uint8_t err_buf[16];
  int16_t r;
  uint8_t err;

  SECTION("network error path");

#ifdef FN_BROKEN_network_read_nb
  SKIP(network_read_nb);
#else
  r = network_read_nb("N7:HTTPS://fujinet.online/", err_buf, sizeof(err_buf));
#if 0
  TEST("network_read_nb on unopened channel returns error", r < 0 || fn_network_error != 0);
#else
  TEST("network_read_nb on unopened channel returns error", r < 0);
#endif // 0
#endif

#ifdef FN_BROKEN_network_open
  SKIP(network_open);
#else
  err = network_open("N1:NOTAPROTOCOL://bad_host/",
                     OPEN_MODE_HTTP_GET, OPEN_TRANS_NONE);
  TEST("network_open with invalid protocol returns error", err != FN_ERR_OK);
#endif

  END_OF_TEST();
}

void test_network_status_after_failed_open(void)
{
  uint8_t err;
  uint16_t bw;
  uint8_t conn, nerr;

  SECTION("network_status after a failed open");

#if defined(FN_BROKEN_network_open) || defined(FN_BROKEN_network_status) \
  || defined(FN_BROKEN_network_close)
  SKIP(network_status_after_failed_open);
#else
  /* A failed open used to lose its reason, so STATUS answered 207
   * (NOT_CONNECTED) whatever went wrong (fujinet-firmware #1704). The open
   * result itself is not checked: IWM acknowledges every open. */
  network_open(NET_MISSING_FILE, OPEN_MODE_READ, OPEN_TRANS_NONE);
  bw = 0; conn = 0; nerr = 0;
  err = network_status(NET_MISSING_FILE, &bw, &conn, &nerr);
  printf("  net_error=%u\n", nerr);
  TEST("status after opening a missing file says file not found",
       err == FN_ERR_OK && nerr == NETWORK_ERROR_FILE_NOT_FOUND);
  network_close(NET_MISSING_FILE);

  network_open(NET_BAD_PROTOCOL, OPEN_MODE_READ, OPEN_TRANS_NONE);
  bw = 0; conn = 0; nerr = 0;
  err = network_status(NET_BAD_PROTOCOL, &bw, &conn, &nerr);
  printf("  net_error=%u\n", nerr);
  TEST("status after opening an unknown protocol says general error",
       err == FN_ERR_OK && nerr == NETWORK_ERROR_GENERAL);
  network_close(NET_BAD_PROTOCOL);
#endif

  END_OF_TEST();
}

uint16_t wait_for_data(const char *net, uint16_t polls)
{
  uint16_t bw = 0, retry;
  uint8_t conn, nerr, err;

  for (retry = 0; retry < polls; retry++) {
    bw = 0; conn = 0; nerr = 0;
    err = network_status(net, &bw, &conn, &nerr);
    if (err != FN_ERR_OK || bw > 0 || nerr != NETWORK_SUCCESS)
      break;
  }

  return bw;
}

void dual_echo_check(const char *net1, const char *net2, const uint8_t *msg)
{
  uint8_t err;
  int16_t r, w;
  char test_name[40];

  w = strlen((const char *) msg);
  err = network_write(net1, msg, w);

  r = wait_for_data(net1, ECHO_WAIT_RETRIES);
  printf("  bytes_waiting=%u\n", r);
  strcpy(test_name, "Nx has bytes waiting");
  test_name[1] = net1[1];
  TEST(test_name, r > 0);

  r = wait_for_data(net2, ECHO_LEAK_POLLS);
  printf("  bytes_waiting=%u\n", r);
  strcpy(test_name, "Nx DOES NOT have bytes waiting");
  test_name[1] = net2[1];
  TEST(test_name, r == 0);

  memset(g.net, 0, sizeof(g.net));
  r = network_read(net1, g.net, w);
  strcpy(test_name, "Nx network_read echoed data");
  test_name[1] = net1[1];
  TEST(test_name, r > 0);
  if (r > 0) {
    if (r > w + 1)
      r = w + 1;
    if (memcmp(g.net, msg, w) != 0)
      cmp_hex("orig", msg, w, "recv", g.net, r);
    strcpy(test_name, "Nx Echo matches sent message");
    test_name[1] = net1[1];
    TEST(test_name, memcmp(g.net, msg, w) == 0);
  }

  return;
}

static void dual_echo_pair(const char *n1, const char *n2)
{
  uint8_t err, err2;
  uint8_t msg[] = ECHO_MSG BASIC_LINE_ENDING;

  err = network_open(n1, OPEN_MODE_RW, OPEN_TRANS_LF);
  err2 = network_open(n2, OPEN_MODE_RW, OPEN_TRANS_LF);
  TEST("dual network_open (RW) succeeds", err == FN_ERR_OK && err2 == FN_ERR_OK);

  // Write a message to N2 and make sure it doesn't come back on N1
  mark_echo_message("DN2", msg);
  dual_echo_check(n2, n1, msg);

  // Do the reverse, write to N1 and validate it is different than N2
  mark_echo_message("DN1", msg);
  dual_echo_check(n1, n2, msg);

  network_close(n1);
  network_close(n2);
}

void test_multiple_network_devices(void)
{
  SECTION("multiple N: devices");
  dual_echo_pair(NET_TCP_SPEC, NET2_ECHO);
  END_OF_TEST();
}

void test_multiple_telnet_devices(void)
{
  SECTION("multiple TELNET N: devices");
  dual_echo_pair(NET_TELNET_SPEC, NET2_TELNET);
  END_OF_TEST();
}

/* Portable substring search (strstr is not everywhere), as in html.c */
static uint8_t t_contains(const char *hay, const char *needle)
{
  uint16_t i, j;

  for (i = 0; hay[i]; i++) {
    for (j = 0; needle[j] && hay[i + j] == needle[j]; j++)
      ;
    if (!needle[j])
      return 1;
  }
  return 0;
}

/* The close came with the line, so STATUS should show it at once. */
#define PEER_CLOSE_POLLS 50

/* Open a daytime server, which sends one line and hangs up. The READ asks
 * for more than the line, so it must stop at what arrived. Then STATUS must
 * show the connection closed with nothing waiting, rather than a line still
 * counted as waiting that no READ can get (fujinet-firmware #2182). */
static void peer_close_check(const char *spec, const char *proto)
{
  uint8_t err, conn, nerr;
  uint16_t bw, retry;
  int16_t r = 0;
  char test_name[48];

  err = network_open(spec, OPEN_MODE_RW, OPEN_TRANS_NONE);
  strcpy(test_name, proto);
  strcat(test_name, " network_open succeeds");
  TEST(test_name, err == FN_ERR_OK);

  bw = wait_for_data(spec, ECHO_WAIT_RETRIES);
  printf("  bytes_waiting=%u\n", bw);
  memset(g.net, 0, sizeof(g.net));
  // network_read() waits until something arrives
  if (bw > 0)
    r = network_read(spec, g.net, sizeof(g.net) - 1);
  printf("  read=%d\n", r);
  strcpy(test_name, proto);
  strcat(test_name, " read gets the line before the close");
  TEST(test_name, r > 0 && (uint16_t) r < sizeof(g.net) - 1
       && t_contains((char *) g.net, DAYTIME_MARK));

  for (retry = 0; retry < PEER_CLOSE_POLLS; retry++) {
    bw = 0; conn = 0; nerr = 0;
    err = network_status(spec, &bw, &conn, &nerr);
    if (err != FN_ERR_OK || (!conn && !bw))
      break;
  }
  printf("  bytes_waiting=%u conn=%u net_error=%u\n", bw, conn, nerr);
  strcpy(test_name, proto);
  strcat(test_name, " status then shows the close");
  TEST(test_name, err == FN_ERR_OK && !conn && !bw);

  network_close(spec);
}

void test_tcp_peer_close(void)
{
  SECTION("TCP peer that hangs up after sending");
#if defined(FN_BROKEN_network_open) || defined(FN_BROKEN_network_read) \
  || defined(FN_BROKEN_network_status)
  SKIP(tcp_peer_close);
#else
  peer_close_check(NET_DAYTIME_TCP, "TCP");
#endif
  END_OF_TEST();
}

void test_telnet_peer_close(void)
{
  SECTION("TELNET peer that hangs up after sending");
#if defined(FN_BROKEN_network_open) || defined(FN_BROKEN_network_read) \
  || defined(FN_BROKEN_network_status)
  SKIP(telnet_peer_close);
#else
  peer_close_check(NET_DAYTIME_TELNET, "TELNET");
#endif
  END_OF_TEST();
}

void test_network_open_no_n_prefix(void)
{
#ifdef FN_BROKEN_network_open_no_n_prefix
  SKIP(network_open_no_n_prefix);
#else
  bool ok;
  uint8_t unit = 2;
  uint16_t url_len;
  char *r;
  char url[] = NET_TCP_SPEC;


  r = strchr(url, ':');
  *(r - 1) = '0' + unit;
  r++;
#if FUJI_VARIABLE_LEN_PACKETS
  url_len = strlen(r);
#else // ! FUJI_VARIABLE_LEN_PACKETS
  url_len = NETWORK_OPEN_LEN;
#endif // FUJI_VARIABLE_LEN_PACKETS
  ok = NETCALL_A1_A2_D(NETCMD_OPEN, unit, OPEN_MODE_RW, OPEN_TRANS_LF, r, url_len);
  TEST("network open without N: succeeds", ok);
  network_close(url);
#endif // FN_BROKEN_network_open_no_n_prefix

  END_OF_TEST();
}

void test_network_status_after_close(void)
{
  uint8_t err;
  uint16_t bw;
  uint8_t conn, nerr;
  int16_t w;
  uint8_t msg[] = ECHO_MSG BASIC_LINE_ENDING;

  SECTION("network_status after close");

#if defined(FN_BROKEN_network_open) || defined(FN_BROKEN_network_write) \
  || defined(FN_BROKEN_network_close)
  SKIP(network_status_after_close);
#else
  /* Setup for the status call below, so the three calls count as one test. */
  mark_echo_message("STC", msg);
  w = strlen((const char *) msg);
  err = network_open(NET_TCP_SPEC, OPEN_MODE_RW, OPEN_TRANS_LF);
  if (err == FN_ERR_OK)
    err = network_write(NET_TCP_SPEC, msg, w);
  if (err == FN_ERR_OK)
    err = network_close(NET_TCP_SPEC);
  TEST("open, write and close before status succeed", err == FN_ERR_OK);
#endif

#ifdef FN_BROKEN_network_status
  SKIP(network_status);
#else
  /* close() unbinds the protocol, so this STATUS runs with none bound. On
   * DriveWire that path skipped transaction_accept() and tripped the NO_GET
   * assertion, aborting the firmware mid-call (fujinet-firmware #1607). */
  bw = 0; conn = 0; nerr = 0;
  err = network_status(NET_TCP_SPEC, &bw, &conn, &nerr);
  TEST("network_status after close returns", err == FN_ERR_OK);
  printf("  bytes_waiting=%u conn=%u net_error=%u\n", bw, conn, nerr);
#endif

#ifndef FN_BROKEN_fuji_get_adapter_config
  /* A device that hit the assertion has rebooted and answers nothing. */
  TEST("FujiNet still responding after status", fuji_get_adapter_config(&g.adapter.ac));
#endif

  END_OF_TEST();
}


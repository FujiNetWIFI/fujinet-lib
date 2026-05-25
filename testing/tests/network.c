#include "broken.h"

#include "harness.h"
#include "constants.h"
#include "globals.h"
#include <fujinet-network.h>

#ifndef _CMOC_VERSION_
#include <stdio.h>
#include <string.h>
#endif /* _CMOC_VERSION_ */

void test_network_init(void)
{
  uint8_t err;

  SECTION("network_init");
#ifdef FN_BROKEN_network_init
  SKIP(network_init);
#endif
  err = network_init();
  TEST("network_init returns FN_ERR_OK", err == FN_ERR_OK);

  END_OF_TEST();
}

void test_network_http_get(void)
{
  uint8_t err;
  uint16_t bw;
  uint8_t conn;
  uint8_t nerr;
  int16_t read_result;

  SECTION("network HTTP GET read");

#ifdef FN_BROKEN_network_open
  SKIP(network_open);
#endif
  err = network_open(NET_DEVICESPEC, OPEN_MODE_HTTP_GET, OPEN_TRANS_LF);
  TEST("network_open (GET) returns FN_ERR_OK", err == FN_ERR_OK);

  bw = 0; conn = 0; nerr = 0;
#ifdef FN_BROKEN_network_status
  SKIP(network_status);
#endif
  err = network_status(NET_DEVICESPEC, &bw, &conn, &nerr);
  TEST("network_status after open succeeds", err == FN_ERR_OK);
  printf("  bytes_waiting=%u conn=%u net_error=%u\n", bw, conn, nerr);

#ifdef FN_BROKEN_network_read
  SKIP(network_read);
#endif
  memset(g.net, 0, sizeof(g.net));
  read_result = network_read(NET_DEVICESPEC, g.net, sizeof(g.net));
  TEST("network_read returns positive byte count", read_result > 0);
  TEST("fn_bytes_read is non-zero", fn_bytes_read > 0);
  printf("  Read %d bytes (fn_bytes_read=%u)\n",
         (int)read_result, fn_bytes_read);

#ifdef FN_BROKEN_network_close
  SKIP(network_close);
#endif
  err = network_close(NET_DEVICESPEC);
  TEST("network_close after GET succeeds", err == FN_ERR_OK);

  END_OF_TEST();
}

void test_network_http_get_nonblocking(void)
{
  uint8_t err;
  uint16_t bw;
  uint8_t conn;
  uint8_t nerr;
  uint16_t to_read;
  int16_t nb_result;

  SECTION("network HTTP GET non-blocking read");

#ifdef FN_BROKEN_network_open
  SKIP(network_open);
#endif
  err = network_open(NET_DEVICESPEC, OPEN_MODE_HTTP_GET, OPEN_TRANS_LF);
  TEST("network_open (GET, nb) returns FN_ERR_OK", err == FN_ERR_OK);

  bw = 0; conn = 0; nerr = 0;
#ifdef FN_BROKEN_network_status
  SKIP(network_status);
#endif
  err = network_status(NET_DEVICESPEC, &bw, &conn, &nerr);
  TEST("network_status (nb) succeeds", err == FN_ERR_OK);

#ifdef FN_BROKEN_network_read_nb
  SKIP(network_read_nb);
#endif
  if (bw > 0) {
    to_read = (bw < (uint16_t)sizeof(g.net))
      ? bw : (uint16_t)sizeof(g.net);
    memset(g.net, 0, sizeof(g.net));
    nb_result = network_read_nb(NET_DEVICESPEC, g.net, to_read);
    TEST("network_read_nb returns non-negative", nb_result >= 0);
    printf("  NB read %d of %u waiting bytes\n", (int)nb_result, bw);
  } else {
    printf("  No bytes waiting yet; skipping non-blocking read assertion\n");
  }

#ifdef FN_BROKEN_network_close
  SKIP(network_close);
#endif
  err = network_close(NET_DEVICESPEC);
  TEST("network_close after nb-GET succeeds", err == FN_ERR_OK);

  END_OF_TEST();
}

void test_network_http_post(void)
{
  uint8_t err;
  int16_t r;
  static const uint8_t bin_data[] = { 0x01, 0x02, 0x03, 0xFF };

  SECTION("network HTTP POST");

#ifdef FN_BROKEN_network_open
  SKIP(network_open);
#endif
  err = network_open(NET_POST_URL, OPEN_MODE_HTTP_POST, OPEN_TRANS_NONE);
  TEST("network_open (POST) returns FN_ERR_OK", err == FN_ERR_OK);

#ifdef FN_BROKEN_network_http_post
  SKIP(network_http_post);
#endif
  err = network_http_post(NET_POST_URL, "{\"test\":\"fujinet_integration\"}");
  TEST("network_http_post succeeds", err == FN_ERR_OK);

#ifdef FN_BROKEN_network_read
  SKIP(network_read);
#endif
  memset(g.net, 0, sizeof(g.net));
  r = network_read(NET_POST_URL, g.net, sizeof(g.net));
  TEST("network_read after POST returns data", r > 0);

#ifdef FN_BROKEN_network_close
  SKIP(network_close);
#endif
  err = network_close(NET_POST_URL);
  TEST("network_close after POST succeeds", err == FN_ERR_OK);

  /* Binary POST variant */
  err = network_open(NET_POST_URL, OPEN_MODE_HTTP_POST, OPEN_TRANS_NONE);
  TEST("network_open for binary POST succeeds", err == FN_ERR_OK);

#ifdef FN_BROKEN_network_http_post_bin
  SKIP(network_http_post_bin);
#endif
  err = network_http_post_bin(NET_POST_URL, bin_data, sizeof(bin_data));
  TEST("network_http_post_bin succeeds", err == FN_ERR_OK);

  memset(g.net, 0, sizeof(g.net));
  r = network_read(NET_POST_URL, g.net, sizeof(g.net));
  TEST("network_read after binary POST returns data", r > 0);

  err = network_close(NET_POST_URL);
  TEST("network_close after binary POST succeeds", err == FN_ERR_OK);

  END_OF_TEST();
}

void test_network_http_headers(void)
{
  uint8_t err;
  int16_t r;

  SECTION("network HTTP custom headers");

#ifdef FN_BROKEN_network_open
  SKIP(network_open);
#endif
  err = network_open(NET_DEVICESPEC, OPEN_MODE_HTTP_GET, OPEN_TRANS_NONE);
  TEST("network_open for header test succeeds", err == FN_ERR_OK);

#ifdef FN_BROKEN_network_http_start_add_headers
  SKIP(network_http_start_add_headers);
#endif
  err = network_http_start_add_headers(NET_DEVICESPEC);
  TEST("network_http_start_add_headers succeeds", err == FN_ERR_OK);

#ifdef FN_BROKEN_network_http_add_header
  SKIP(network_http_add_header);
#endif
  err = network_http_add_header(NET_DEVICESPEC, "X-FujiNet-Test: integration");
  TEST("network_http_add_header succeeds", err == FN_ERR_OK);

  err = network_http_add_header(NET_DEVICESPEC, "Accept: text/html");
  TEST("Second network_http_add_header succeeds", err == FN_ERR_OK);

#ifdef FN_BROKEN_network_http_end_add_headers
  SKIP(network_http_end_add_headers);
#endif
  err = network_http_end_add_headers(NET_DEVICESPEC);
  TEST("network_http_end_add_headers succeeds", err == FN_ERR_OK);

#ifdef FN_BROKEN_network_read
  SKIP(network_read);
#endif
  memset(g.net, 0, sizeof(g.net));
  r = network_read(NET_DEVICESPEC, g.net, sizeof(g.net));
  TEST("network_read with custom headers returns data", r > 0);

#ifdef FN_BROKEN_network_close
  SKIP(network_close);
#endif
  err = network_close(NET_DEVICESPEC);
  TEST("network_close after header test succeeds", err == FN_ERR_OK);

  END_OF_TEST();
}

void test_network_http_channel_mode(void)
{
  uint8_t err;

  SECTION("network HTTP channel mode");

#ifdef FN_BROKEN_network_open
  SKIP(network_open);
#endif
  err = network_open(NET_DEVICESPEC, OPEN_MODE_HTTP_GET, OPEN_TRANS_NONE);
  TEST("network_open for channel mode test succeeds", err == FN_ERR_OK);

#ifdef FN_BROKEN_network_http_set_channel_mode
  SKIP(network_http_set_channel_mode);
#endif
  err = network_http_set_channel_mode(NET_DEVICESPEC,
                                      HTTP_CHAN_MODE_COLLECT_HEADERS);
  TEST("network_http_set_channel_mode(COLLECT_HEADERS) succeeds", err == FN_ERR_OK);

  err = network_http_set_channel_mode(NET_DEVICESPEC, HTTP_CHAN_MODE_BODY);
  TEST("network_http_set_channel_mode(BODY) succeeds", err == FN_ERR_OK);

#ifdef FN_BROKEN_network_close
  SKIP(network_close);
#endif
  err = network_close(NET_DEVICESPEC);
  TEST("network_close after channel mode test succeeds", err == FN_ERR_OK);

  END_OF_TEST();
}

void test_network_json(void)
{
  uint8_t err;
  char result[64];
  int16_t n;

  SECTION("network JSON parse and query");

#ifdef FN_BROKEN_network_open
  SKIP(network_open);
#endif
  err = network_open(NET_JSON_URL, OPEN_MODE_HTTP_GET, OPEN_TRANS_NONE);
  TEST("network_open for JSON succeeds", err == FN_ERR_OK);

#ifdef FN_BROKEN_network_json_parse
  SKIP(network_json_parse);
#endif
  err = network_json_parse(NET_JSON_URL);
  TEST("network_json_parse succeeds", err == FN_ERR_OK);

#ifdef FN_BROKEN_network_json_query
  SKIP(network_json_query);
#endif
  memset(result, 0, sizeof(result));
  n = network_json_query(NET_JSON_URL, "/slideshow/title", result);
  TEST("network_json_query returns data", n > 0);
  TEST("JSON query result non-empty", result[0] != '\0');
  printf("  JSON /slideshow/title = %s\n", result);

#ifdef FN_BROKEN_network_close
  SKIP(network_close);
#endif
  err = network_close(NET_JSON_URL);
  TEST("network_close after JSON succeeds", err == FN_ERR_OK);

  END_OF_TEST();
}

void test_network_http_put_delete(void)
{
  uint8_t err;
  int16_t r;

  SECTION("network HTTP PUT and DELETE");

#ifdef FN_BROKEN_network_open
  SKIP(network_open);
#endif
  err = network_open(NET_POST_URL, OPEN_MODE_HTTP_PUT, OPEN_TRANS_NONE);
  TEST("network_open (PUT) succeeds", err == FN_ERR_OK);

#ifdef FN_BROKEN_network_http_put
  SKIP(network_http_put);
#endif
  err = network_http_put(NET_POST_URL, "fujinet=put_test");
  TEST("network_http_put succeeds", err == FN_ERR_OK);

#ifdef FN_BROKEN_network_close
  SKIP(network_close);
#endif
  err = network_close(NET_POST_URL);
  TEST("network_close after PUT succeeds", err == FN_ERR_OK);

#ifdef FN_BROKEN_network_http_delete
  SKIP(network_http_delete);
#endif
  err = network_http_delete(NET_DELETE_URL, OPEN_TRANS_NONE);
  TEST("network_http_delete succeeds", err == FN_ERR_OK);

#ifdef FN_BROKEN_network_read
  SKIP(network_read);
#endif
  memset(g.net, 0, sizeof(g.net));
  r = network_read(NET_DELETE_URL, g.net, sizeof(g.net));
  TEST("network_read after DELETE returns data", r > 0);

  err = network_close(NET_DELETE_URL);
  TEST("network_close after DELETE succeeds", err == FN_ERR_OK);

  END_OF_TEST();
}

void test_network_write(void)
{
  uint8_t err;
  int16_t r;
  static const uint8_t msg[] = "FujiNet integration test\n";

  SECTION("network_write (raw TCP)");

#ifdef FN_BROKEN_network_open
  SKIP(network_open);
#endif
  err = network_open(NET_TCP_SPEC, OPEN_MODE_RW,
#ifdef BUILD_ATARI
                     OPEN_TRANS_LF
#else
#warning "OPEN_TRANS_LF should work on ALL platforms"
                     OPEN_TRANS_NONE
#endif // BUILD_ATARI
                     );
  TEST("network_open (TCP RW) succeeds", err == FN_ERR_OK);

#ifdef FN_BROKEN_network_write
  SKIP(network_write);
#endif
  err = network_write(NET_TCP_SPEC, msg, sizeof(msg) - 1);
  TEST("network_write succeeds", err == FN_ERR_OK);

#ifdef FN_BROKEN_network_read
  SKIP(network_read);
#endif
  memset(g.net, 0, sizeof(g.net));
  r = network_read(NET_TCP_SPEC, g.net, sizeof(msg) - 1);
  TEST("network_read echoed data", r > 0);
  if (r > 0) {
    TEST("Echo matches sent message", memcmp(g.net, msg, r) == 0);
  }

#ifdef FN_BROKEN_network_close
  SKIP(network_close);
#endif
  err = network_close(NET_TCP_SPEC);
  TEST("network_close TCP succeeds", err == FN_ERR_OK);

  END_OF_TEST();
}

void test_network_unit(void)
{
  SECTION("network_unit helper");
#ifdef FN_BROKEN_network_unit
  SKIP(network_unit);
#endif
  TEST("network_unit('N1:...') == 1", network_unit("N1:HTTPS://fujinet.online/") == 1);
  TEST("network_unit('N2:...') == 2", network_unit("N2:HTTPS://fujinet.online/") == 2);
  TEST("network_unit('N8:...') == 8", network_unit("N8:HTTPS://fujinet.online/") == 8);

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
#endif
  r = network_read_nb("N7:HTTPS://fujinet.online/", err_buf, sizeof(err_buf));
  TEST("network_read_nb on unopened channel returns error", r < 0 || fn_network_error != 0);

#ifdef FN_BROKEN_network_open
  SKIP(network_open);
#endif
  err = network_open("N1:NOTAPROTOCOL://bad_host/",
                     OPEN_MODE_HTTP_GET, OPEN_TRANS_NONE);
  TEST("network_open with invalid protocol returns error", err != FN_ERR_OK);

  END_OF_TEST();
}

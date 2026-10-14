/**
 * @file http.c
 * @brief HTTP(S) and JSON channel tests against httpbin.
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
#else
  err = network_open(NET_DEVICESPEC, OPEN_MODE_HTTP_GET, OPEN_TRANS_LF);
  TEST("network_open (GET) returns FN_ERR_OK", err == FN_ERR_OK);
#endif

  bw = 0; conn = 0; nerr = 0;
#ifdef FN_BROKEN_network_status
  SKIP(network_status);
#else
  err = network_status(NET_DEVICESPEC, &bw, &conn, &nerr);
  TEST("network_status after open succeeds", err == FN_ERR_OK);
  printf("  bytes_waiting=%u conn=%u net_error=%u\n", bw, conn, nerr);
  TEST("network no error", nerr == NETWORK_SUCCESS);
#endif

#ifdef FN_BROKEN_network_read
  SKIP(network_read);
#else
  memset(g.net, 0, sizeof(g.net));
  read_result = network_read(NET_DEVICESPEC, g.net, sizeof(g.net));
  TEST("network_read returns positive byte count", read_result > 0);
#if 0
  TEST("fn_bytes_read is non-zero", fn_bytes_read > 0);
  printf("  Read %d bytes (fn_bytes_read=%u)\n",
         (int)read_result, fn_bytes_read);
#else
  printf("  Read %d bytes\n", (int)read_result);
#endif // 0
#endif

#ifdef FN_BROKEN_network_close
  SKIP(network_close);
#else
  err = network_close(NET_DEVICESPEC);
  TEST("network_close after GET succeeds", err == FN_ERR_OK);
#endif

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
#else
  err = network_open(NET_DEVICESPEC, OPEN_MODE_HTTP_GET, OPEN_TRANS_LF);
  TEST("network_open (GET, nb) returns FN_ERR_OK", err == FN_ERR_OK);
#endif

  bw = 0; conn = 0; nerr = 0;
#ifdef FN_BROKEN_network_status
  SKIP(network_status);
#else
  err = network_status(NET_DEVICESPEC, &bw, &conn, &nerr);
  TEST("network_status (nb) succeeds", err == FN_ERR_OK);
  printf("  bytes_waiting=%u conn=%u net_error=%u\n", bw, conn, nerr);
  TEST("network no error", nerr == NETWORK_SUCCESS);
#endif

#ifdef FN_BROKEN_network_read_nb
  SKIP(network_read_nb);
#else
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
#endif

#ifdef FN_BROKEN_network_close
  SKIP(network_close);
#else
  err = network_close(NET_DEVICESPEC);
  TEST("network_close after nb-GET succeeds", err == FN_ERR_OK);
#endif

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
#else
  err = network_open(NET_POST_URL, OPEN_MODE_HTTP_POST, OPEN_TRANS_NONE);
  TEST("network_open (POST) returns FN_ERR_OK", err == FN_ERR_OK);
#endif

#ifdef FN_BROKEN_network_http_post
  SKIP(network_http_post);
#else
  err = network_http_post(NET_POST_URL, "{\"test\":\"fujinet_integration\"}");
  TEST("network_http_post succeeds", err == FN_ERR_OK);
#endif

#ifdef FN_BROKEN_network_read
  SKIP(network_read);
#else
  memset(g.net, 0, sizeof(g.net));
  r = network_read(NET_POST_URL, g.net, sizeof(g.net));
  TEST("network_read after POST returns data", r > 0);
#endif

#ifdef FN_BROKEN_network_close
  SKIP(network_close);
#else
  err = network_close(NET_POST_URL);
  TEST("network_close after POST succeeds", err == FN_ERR_OK);

  /* Binary POST variant */
  err = network_open(NET_POST_URL, OPEN_MODE_HTTP_POST, OPEN_TRANS_NONE);
  TEST("network_open for binary POST succeeds", err == FN_ERR_OK);
#endif

#ifdef FN_BROKEN_network_http_post_bin
  SKIP(network_http_post_bin);
#else
  err = network_http_post_bin(NET_POST_URL, bin_data, sizeof(bin_data));
  TEST("network_http_post_bin succeeds", err == FN_ERR_OK);

  memset(g.net, 0, sizeof(g.net));
  r = network_read(NET_POST_URL, g.net, sizeof(g.net));
  TEST("network_read after binary POST returns data", r > 0);

  err = network_close(NET_POST_URL);
  TEST("network_close after binary POST succeeds", err == FN_ERR_OK);
#endif

  END_OF_TEST();
}

#define LARGE_POST_LEN 1200
static char large_post[LARGE_POST_LEN + 1];

void test_network_http_post_large(void)
{
  uint8_t err;
  int16_t n;
  uint16_t bw;
  uint8_t conn, nerr;

  SECTION("network HTTP POST larger than a bus packet");

#if defined(FN_BROKEN_network_http_post) || defined(FN_BROKEN_network_json_query)
  SKIP(network_http_post_large);
#else
  memset(large_post, 'Q', LARGE_POST_LEN);
  large_post[LARGE_POST_LEN] = '\0';

  err = network_open(NET_POST_URL, OPEN_MODE_HTTP_POST, OPEN_TRANS_NONE);
  TEST("network_open (POST) returns FN_ERR_OK", err == FN_ERR_OK);
  /* A bus that copies the whole write into one fixed packet overruns it (#145). */
  err = network_http_post(NET_POST_URL, large_post);
  TEST("POST of 1200 bytes succeeds", err == FN_ERR_OK);

  /* httpbin.dev echoes headers as arrays and an octet-stream body base64
   * encoded, so check what it received. */
  err = network_json_parse(NET_POST_URL);
  TEST("network_json_parse of the echo succeeds", err == FN_ERR_OK);
  memset(g.net, 0, sizeof(g.net));
  n = network_json_query(NET_POST_URL, "/headers/Content-Length/0", (char *)g.net);
  printf("  server received %s bytes\n", (char *)g.net);
  TEST("server received all 1200 bytes", n > 0 && strcmp((char *)g.net, "1200") == 0);

  bw = 0; conn = 0; nerr = 0;
  err = network_status(NET_POST_URL, &bw, &conn, &nerr);
  TEST("device still answers STATUS", err == FN_ERR_OK);

  network_close(NET_POST_URL);
#endif

  END_OF_TEST();
}

void test_network_http_headers(void)
{
  uint8_t err;
  int16_t r;

  SECTION("network HTTP custom headers");

#ifdef FN_BROKEN_network_open
  SKIP(network_open);
#else
  err = network_open(NET_DEVICESPEC, OPEN_MODE_HTTP_GET, OPEN_TRANS_NONE);
  TEST("network_open for header test succeeds", err == FN_ERR_OK);
#endif

#ifdef FN_BROKEN_network_http_start_add_headers
  SKIP(network_http_start_add_headers);
#else
  err = network_http_start_add_headers(NET_DEVICESPEC);
  TEST("network_http_start_add_headers succeeds", err == FN_ERR_OK);
#endif

#ifdef FN_BROKEN_network_http_add_header
  SKIP(network_http_add_header);
#else
  err = network_http_add_header(NET_DEVICESPEC, "X-FujiNet-Test: integration");
  TEST("network_http_add_header succeeds", err == FN_ERR_OK);

  err = network_http_add_header(NET_DEVICESPEC, "Accept: text/html");
  TEST("Second network_http_add_header succeeds", err == FN_ERR_OK);
#endif

#ifdef FN_BROKEN_network_http_end_add_headers
  SKIP(network_http_end_add_headers);
#else
  err = network_http_end_add_headers(NET_DEVICESPEC);
  TEST("network_http_end_add_headers succeeds", err == FN_ERR_OK);
#endif

#ifdef FN_BROKEN_network_read
  SKIP(network_read);
#else
  memset(g.net, 0, sizeof(g.net));
  r = network_read(NET_DEVICESPEC, g.net, sizeof(g.net));
  TEST("network_read with custom headers returns data", r > 0);
#endif

#ifdef FN_BROKEN_network_close
  SKIP(network_close);
#else
  err = network_close(NET_DEVICESPEC);
  TEST("network_close after header test succeeds", err == FN_ERR_OK);
#endif

  END_OF_TEST();
}

void test_network_http_channel_mode(void)
{
  uint8_t err;

  SECTION("network HTTP channel mode");

#ifdef FN_BROKEN_network_open
  SKIP(network_open);
#else
  err = network_open(NET_DEVICESPEC, OPEN_MODE_HTTP_GET, OPEN_TRANS_NONE);
  TEST("network_open for channel mode test succeeds", err == FN_ERR_OK);
#endif

#ifdef FN_BROKEN_network_http_set_channel_mode
  SKIP(network_http_set_channel_mode);
#else
  err = network_http_set_channel_mode(NET_DEVICESPEC,
                                      HTTP_CHAN_MODE_COLLECT_HEADERS);
  TEST("network_http_set_channel_mode(COLLECT_HEADERS) succeeds", err == FN_ERR_OK);

  err = network_http_set_channel_mode(NET_DEVICESPEC, HTTP_CHAN_MODE_BODY);
  TEST("network_http_set_channel_mode(BODY) succeeds", err == FN_ERR_OK);
#endif

#ifdef FN_BROKEN_network_close
  SKIP(network_close);
#else
  err = network_close(NET_DEVICESPEC);
  TEST("network_close after channel mode test succeeds", err == FN_ERR_OK);
#endif

  END_OF_TEST();
}

void test_network_json(void)
{
  uint8_t err;
  char result[64];
  int16_t n;
  uint16_t bw;
  uint8_t conn, nerr;

  SECTION("network JSON parse and query");

#ifdef FN_BROKEN_network_open
  SKIP(network_open);
#else
  err = network_open(NET_JSON_URL, OPEN_MODE_HTTP_GET, OPEN_TRANS_NONE);
  TEST("network_open for JSON succeeds", err == FN_ERR_OK);
#endif

#ifdef FN_BROKEN_network_json_parse
  SKIP(network_json_parse);
#else
  err = network_json_parse(NET_JSON_URL);
  TEST("network_json_parse succeeds", err == FN_ERR_OK);
#endif

  bw = 0; conn = 0; nerr = 0;
#ifdef FN_BROKEN_network_status
  SKIP(network_status);
#else
  err = network_status(NET_DEVICESPEC, &bw, &conn, &nerr);
  TEST("network_status after parse succeeds", err == FN_ERR_OK);
  printf("  bytes_waiting=%u conn=%u net_error=%u\n", bw, conn, nerr);
  TEST("network no error", nerr == NETWORK_SUCCESS || nerr == NETWORK_ERROR_END_OF_FILE);
#endif

#ifdef FN_BROKEN_network_json_query
  SKIP(network_json_query);
#else
  memset(result, 0, sizeof(result));
  n = network_json_query(NET_JSON_URL, "/slideshow/title", result);
  TEST("network_json_query returns data", n > 0);
  TEST("JSON query result non-empty", result[0] != '\0');
  printf("  JSON /slideshow/title = %s\n", result);
#endif

#ifdef FN_BROKEN_network_close
  SKIP(network_close);
#else
  err = network_close(NET_JSON_URL);
  TEST("network_close after JSON succeeds", err == FN_ERR_OK);
#endif

  END_OF_TEST();
}

void test_network_http_put_plain(void)
{
  uint8_t err;
  int16_t r;

  SECTION("network HTTP PUT without headers");

#ifdef FN_BROKEN_network_open
  SKIP(network_open);
#else
  err = network_open(NET_PUT_URL, OPEN_MODE_HTTP_PUT, OPEN_TRANS_NONE);
  TEST("network_open (plain PUT) succeeds", err == FN_ERR_OK);
#endif

#ifdef FN_BROKEN_network_http_put
  SKIP(network_http_put);
#else
  err = network_http_put(NET_PUT_URL, "fujinet=put_test");
  TEST("network_http_put succeeds", err == FN_ERR_OK);
#endif

#ifdef FN_BROKEN_network_read
  SKIP(network_read);
#else
  /* The calls above report success even when the body never reaches the
   * server, so the response is what proves the request was sent. */
  memset(g.net, 0, sizeof(g.net));
  r = network_read(NET_PUT_URL, g.net, sizeof(g.net));
  TEST("network_read after plain PUT returns data", r > 0);
#endif

#ifdef FN_BROKEN_network_close
  SKIP(network_close);
#else
  err = network_close(NET_PUT_URL);
  TEST("network_close after plain PUT succeeds", err == FN_ERR_OK);
#endif

  END_OF_TEST();
}

void test_network_http_put_delete(void)
{
  uint8_t err;
  int16_t r;

  SECTION("network HTTP PUT and DELETE");

#ifdef FN_BROKEN_network_open
  SKIP(network_open);
#else
  err = network_open(NET_PUT_URL, OPEN_MODE_HTTP_PUT_H, OPEN_TRANS_NONE);
  TEST("network_open (PUT) succeeds", err == FN_ERR_OK);
#endif

#ifdef FN_BROKEN_network_http_put
  SKIP(network_http_put);
#else
  err = network_http_put(NET_PUT_URL, "fujinet=put_test");
  TEST("network_http_put succeeds", err == FN_ERR_OK);
#endif

#ifdef FN_BROKEN_network_close
  SKIP(network_close);
#else
  err = network_close(NET_PUT_URL);
  TEST("network_close after PUT succeeds", err == FN_ERR_OK);
#endif

#ifdef FN_BROKEN_network_http_delete
  SKIP(network_http_delete);
#else
  err = network_http_delete(NET_DELETE_URL, OPEN_TRANS_NONE);
  TEST("network_http_delete succeeds", err == FN_ERR_OK);
#endif

#ifdef FN_BROKEN_network_read
  SKIP(network_read);
#else
  memset(g.net, 0, sizeof(g.net));
  r = network_read(NET_DELETE_URL, g.net, sizeof(g.net));
  TEST("network_read after DELETE returns data", r > 0);

  err = network_close(NET_DELETE_URL);
  TEST("network_close after DELETE succeeds", err == FN_ERR_OK);
#endif

  END_OF_TEST();
}

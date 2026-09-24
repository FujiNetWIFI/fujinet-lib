/**
 * @file parse_errors.c
 * @brief NDevice parser error codes: what NET_SET_PARSER, NET_PARSE,
 *        NET_QUERY and NET_SET_PARAMETER report when they fail, and what
 *        the following STATUS says.
 *
 * Firmware work exercised here (fujinet-firmware):
 *   PR #1640 / issue #1417  "report JSON/HTML parse failures instead of
 *   success". Three stacked layers dropped the parse result --
 *   NDevice::fujidev_do_parse, JSONParser::parse and HTMLParser::parse --
 *   so a failed parse looked like a success on the wire. The same PR adds
 *   the null-_parser guards to fujidev_do_parse and fujidev_set_query (a
 *   NET_PARSE with no channel open used to SIGSEGV the firmware) and a
 *   sticky NParser::_parseError so a later STATUS can say *why* the channel
 *   is empty.
 *
 * Expect these to fail against pre-#1640 firmware: that is the point.
 * test_parse_no_parser and test_parse_after_close can reboot an unpatched
 * device instead of failing cleanly, which is what the TEST_ALIVE lines
 * detect.
 *
 * Error codes asserted (fujinet-err.h, firmware status_error_codes.h):
 *   213 COULD_NOT_PARSE_JSON  malformed or empty body in JSON mode
 *   144 GENERAL               empty body in HTML mode
 *   207 NOT_CONNECTED         STATUS with no protocol bound
 */

#include "broken.h"

#include "parse_errors.h"
#include "harness.h"
#include "constants.h"
#include "globals.h"
#include <fujinet-network.h>

#ifndef _CMOC_VERSION_
#include <stdio.h>
#include <string.h>
#endif /* _CMOC_VERSION_ */

#if defined(FN_BROKEN_network_open) || defined(FN_BROKEN_network_close) \
  || defined(FN_BROKEN_network_status)
#define FN_BROKEN_parse_errors
#endif

/*
 * A device that hit the pre-#1640 SIGSEGV has rebooted and answers nothing.
 * Same idiom as test_network_status_after_close in network.c.
 */
#define TEST_ALIVE(name) TEST(name, fuji_get_adapter_config(&g.adapter.ac))

/* Read back the device STATUS error byte. 0 is not a valid FujiNet error
 * code, so it doubles as "the STATUS call itself failed". */
static uint8_t status_err(const char *spec)
{
  uint16_t bw;
  uint8_t conn, nerr;

  bw = 0; conn = 0; nerr = 0;
  if (network_status(spec, &bw, &conn, &nerr) != FN_ERR_OK)
    return 0;
  printf("  bytes_waiting=%u conn=%u net_error=%u\n",
         (unsigned) bw, (unsigned) conn, (unsigned) nerr);
  return nerr;
}

/* NET_PARSE on its own, with no NET_SET_PARSER in front of it.
 *
 * network_json_parse() cannot reach this: it sends SET_PARSER itself, and
 * fujidev_set_parser happily builds a JSONParser around a null protocol. The
 * null-_parser guard only sees a bare NET_PARSE. */
#ifndef FN_BROKEN_bare_net_parse
static bool bare_parse(const char *spec)
{
  return NETCALL(NETCMD_PARSE, network_unit(spec));
}
#endif /* FN_BROKEN_bare_net_parse */

void test_parse_no_parser(void)
{
  bool ok;
  uint8_t nerr;

  SECTION("NET_PARSE with no channel open");

#if defined(FN_BROKEN_parse_errors) || defined(FN_BROKEN_bare_net_parse)
  SKIP(parse_errors);
#else
  /* Whatever ran before us, make sure unit 1 has nothing bound. close()
   * nulls both _protocol and _parser. */
  network_close(NET_JSON_URL);

  ok = bare_parse(NET_JSON_URL);
  TEST("bare NET_PARSE with no channel reports an error", !ok);

  /* The pre-#1640 crash: fujidev_do_parse dereferenced a null _parser. */
  TEST_ALIVE("FujiNet still responding after NET_PARSE with no channel");

  nerr = status_err(NET_JSON_URL);
  TEST("STATUS with no protocol reports NOT_CONNECTED",
       nerr == NETWORK_ERROR_NOT_CONNECTED);
#endif /* FN_BROKEN_parse_errors */

  END_OF_TEST();
}

void test_parse_after_close(void)
{
  uint8_t err;
  bool ok;
  uint8_t nerr;

  SECTION("NET_PARSE after NET_CLOSE");

#if defined(FN_BROKEN_parse_errors) || defined(FN_BROKEN_network_json_parse) \
  || defined(FN_BROKEN_bare_net_parse)
  SKIP(parse_errors);
#else
  err = network_open(NET_JSON_URL, OPEN_MODE_HTTP_GET, OPEN_TRANS_NONE);
  TEST("open JSON URL", err == FN_ERR_OK);

  err = network_json_parse(NET_JSON_URL);
  TEST("JSON parse of a good body succeeds", err == FN_ERR_OK);

  err = network_close(NET_JSON_URL);
  TEST("close JSON URL", err == FN_ERR_OK);

  /* close() drops _parser as well as _protocol, so this is the null-_parser
   * path again, reached the way a real client reaches it. */
  ok = bare_parse(NET_JSON_URL);
  TEST("bare NET_PARSE after close reports an error", !ok);
  TEST_ALIVE("FujiNet still responding after NET_PARSE post-close");

  nerr = status_err(NET_JSON_URL);
  TEST("STATUS after close reports NOT_CONNECTED",
       nerr == NETWORK_ERROR_NOT_CONNECTED);
#endif /* FN_BROKEN_parse_errors */

  END_OF_TEST();
}

void test_parse_parser_none(void)
{
  uint8_t err;
  bool ok;

  SECTION("NET_PARSE in PARSER_NONE mode");

#if defined(FN_BROKEN_parse_errors) || defined(FN_BROKEN_network_set_parser) \
  || defined(FN_BROKEN_bare_net_parse)
  SKIP(parse_errors);
#else
  err = network_open(NET_JSON_URL, OPEN_MODE_HTTP_GET, OPEN_TRANS_NONE);
  TEST("open JSON URL", err == FN_ERR_OK);

  err = network_set_parser(NET_JSON_URL, PARSER_NONE);
  TEST("set PARSER_NONE", err == FN_ERR_OK);

  /* fujidev_open installs a plain NParser, whose parse() has always returned
   * error; before #1640 NDevice threw that result away. A client that skips
   * NET_SET_PARSER now gets a diagnosable failure instead of a silent no-op. */
  ok = bare_parse(NET_JSON_URL);
  TEST("NET_PARSE without a parser reports an error", !ok);

  /* No STATUS assertion: NParser::parse() leaves _parseError alone, so the
   * byte here is whatever the protocol reports, not a parse code. */
  status_err(NET_JSON_URL);

  err = network_close(NET_JSON_URL);
  TEST("close JSON URL", err == FN_ERR_OK);
#endif /* FN_BROKEN_parse_errors */

  END_OF_TEST();
}

void test_json_parse_malformed(void)
{
  uint8_t err;
  uint8_t nerr;

  SECTION("JSON parse of a non-JSON body");

#if defined(FN_BROKEN_parse_errors) || defined(FN_BROKEN_network_json_parse)
  SKIP(network_json_parse);
#else
  /* NET_HTML_URL is httpbin's fixed HTML page: a perfectly good body that
   * cJSON cannot parse. */
  err = network_open(NET_HTML_URL, OPEN_MODE_HTTP_GET, OPEN_TRANS_NONE);
  TEST("open HTML page", err == FN_ERR_OK);

  err = network_json_parse(NET_HTML_URL);
  TEST("JSON parse of HTML reports an error", err != FN_ERR_OK);

  /* The assertion that matters, and the one that works on every bus. */
  nerr = status_err(NET_HTML_URL);
  TEST("STATUS reports COULD_NOT_PARSE_JSON",
       nerr == NETWORK_ERROR_COULD_NOT_PARSE_JSON);

  TEST_ALIVE("FujiNet still responding after a failed JSON parse");

  err = network_close(NET_HTML_URL);
  TEST("close HTML page", err == FN_ERR_OK);
#endif /* FN_BROKEN_parse_errors */

  END_OF_TEST();
}

void test_json_parse_empty_body(void)
{
  uint8_t err;
  uint8_t nerr;

  SECTION("JSON parse of an empty body");

#if defined(FN_BROKEN_parse_errors) || defined(FN_BROKEN_network_json_parse)
  SKIP(network_json_parse);
#else
  /* FNJSON::parse() conflates "empty body" with "malformed JSON", so a 204
   * now fails NET_PARSE where it used to report success. This is #1640's
   * most visible behavior change. */
  err = network_open(NET_EMPTY_URL, OPEN_MODE_HTTP_GET, OPEN_TRANS_NONE);
  TEST("open 204 URL", err == FN_ERR_OK);

  err = network_json_parse(NET_EMPTY_URL);
  TEST("JSON parse of an empty body reports an error",
               err != FN_ERR_OK);

  nerr = status_err(NET_EMPTY_URL);
  TEST("STATUS reports COULD_NOT_PARSE_JSON for an empty body",
       nerr == NETWORK_ERROR_COULD_NOT_PARSE_JSON);

  err = network_close(NET_EMPTY_URL);
  TEST("close 204 URL", err == FN_ERR_OK);
#endif /* FN_BROKEN_parse_errors */

  END_OF_TEST();
}

void test_html_parse_empty_body(void)
{
  uint8_t err;
  uint8_t nerr;

  SECTION("HTML parse of an empty body");

#if defined(FN_BROKEN_parse_errors) || defined(FN_BROKEN_network_html_parse)
  SKIP(network_html_parse);
#else
  /* Gumbo does HTML5 error recovery, so a malformed body still parses; an
   * empty one is the only way to fail FNHTML::parse(). HTMLParser reports
   * GENERAL rather than the JSON-specific 213. */
  err = network_open(NET_EMPTY_URL, OPEN_MODE_HTTP_GET, OPEN_TRANS_NONE);
  TEST("open 204 URL", err == FN_ERR_OK);

  err = network_html_parse(NET_EMPTY_URL);
  TEST("HTML parse of an empty body reports an error",
               err != FN_ERR_OK);

  nerr = status_err(NET_EMPTY_URL);
  TEST("STATUS reports GENERAL for a failed HTML parse",
       nerr == NETWORK_ERROR_GENERAL);

  TEST_ALIVE("FujiNet still responding after a failed HTML parse");

  err = network_close(NET_EMPTY_URL);
  TEST("close 204 URL", err == FN_ERR_OK);
#endif /* FN_BROKEN_parse_errors */

  END_OF_TEST();
}

void test_parse_error_not_sticky(void)
{
  uint8_t err;
  uint8_t nerr;
  int16_t n;

  SECTION("parse error does not leak into the next channel");

#if defined(FN_BROKEN_parse_errors) || defined(FN_BROKEN_network_json_parse) \
  || defined(FN_BROKEN_network_json_query)
  SKIP(network_json_parse);
#else
  /* Poison unit 1 with a failed parse... */
  err = network_open(NET_HTML_URL, OPEN_MODE_HTTP_GET, OPEN_TRANS_NONE);
  TEST("open HTML page", err == FN_ERR_OK);
  network_json_parse(NET_HTML_URL);
  nerr = status_err(NET_HTML_URL);
  TEST("STATUS is poisoned with 213",
       nerr == NETWORK_ERROR_COULD_NOT_PARSE_JSON);
  err = network_close(NET_HTML_URL);
  TEST("close HTML page", err == FN_ERR_OK);

  /* ...then check the next channel on the same unit is clean. This is also
   * the happy path #1640 must not have broken. */
  err = network_open(NET_JSON_URL, OPEN_MODE_HTTP_GET, OPEN_TRANS_NONE);
  TEST("open JSON URL", err == FN_ERR_OK);

  err = network_json_parse(NET_JSON_URL);
  TEST("JSON parse of a good body still succeeds", err == FN_ERR_OK);

  nerr = status_err(NET_JSON_URL);
  TEST("STATUS is clean after a good parse",
       nerr == NETWORK_SUCCESS || nerr == NETWORK_ERROR_END_OF_FILE);

  memset(g.net, 0, sizeof(g.net));
  n = network_json_query(NET_JSON_URL, "/slideshow/title", (char *) g.net);
  printf("  /slideshow/title=\"%s\" (n=%d)\n", (char *) g.net, (int) n);
  TEST("query still returns data", n > 0);

  err = network_close(NET_JSON_URL);
  TEST("close JSON URL", err == FN_ERR_OK);
#endif /* FN_BROKEN_parse_errors */

  END_OF_TEST();
}

void test_query_after_failed_parse(void)
{
  uint8_t err;
  uint8_t nerr;
  int16_t n;

  SECTION("NET_QUERY after a failed parse");

#if defined(FN_BROKEN_parse_errors) || defined(FN_BROKEN_network_json_parse) \
  || defined(FN_BROKEN_network_json_query)
  SKIP(network_json_query);
#else
  err = network_open(NET_HTML_URL, OPEN_MODE_HTTP_GET, OPEN_TRANS_NONE);
  TEST("open HTML page", err == FN_ERR_OK);

  network_json_parse(NET_HTML_URL);

  /* JSONParser::setQuery() returns success unconditionally and cJSON's
   * pointer lookup is null-safe, so the query does not error -- it just has
   * nothing to give back. Pinning current behavior: if this ever starts
   * returning data, something is reading a stale document. */
  memset(g.net, 0, sizeof(g.net));
  n = network_json_query(NET_HTML_URL, "/slideshow/title", (char *) g.net);
  printf("  n=%d\n", (int) n);
  TEST("query after a failed parse returns no data", n <= 0);

  /* The parse error is still what STATUS reports; a query does not clear it. */
  nerr = status_err(NET_HTML_URL);
  TEST("STATUS still reports COULD_NOT_PARSE_JSON after the query",
       nerr == NETWORK_ERROR_COULD_NOT_PARSE_JSON);

  TEST_ALIVE("FujiNet still responding after querying a failed parse");

  err = network_close(NET_HTML_URL);
  TEST("close HTML page", err == FN_ERR_OK);
#endif /* FN_BROKEN_parse_errors */

  END_OF_TEST();
}

void test_query_no_channel(void)
{
  uint8_t nerr;
  int16_t n;

  SECTION("NET_QUERY with no channel open");

#if defined(FN_BROKEN_parse_errors) || defined(FN_BROKEN_network_json_query)
  SKIP(network_json_query);
#else
  network_close(NET_JSON_URL);

  /* fujidev_set_query drains the WILL_GET payload *before* it checks
   * _parser, otherwise the bus desyncs and every later command lands on the
   * wrong byte. The status call below is what proves the drain happened. */
  memset(g.net, 0, sizeof(g.net));
  n = network_json_query(NET_JSON_URL, "/slideshow/title", (char *) g.net);
  printf("  n=%d\n", (int) n);
  TEST("query with no channel returns no data", n <= 0);
  TEST("query with no channel reports an error", n < 0);

  nerr = status_err(NET_JSON_URL);
  TEST("bus still in sync: STATUS reports NOT_CONNECTED",
       nerr == NETWORK_ERROR_NOT_CONNECTED);

  TEST_ALIVE("FujiNet still responding after NET_QUERY with no channel");
#endif /* FN_BROKEN_parse_errors */

  END_OF_TEST();
}

void test_set_parser_invalid_mode(void)
{
  uint8_t err;

  SECTION("NET_SET_PARSER with an invalid mode");

#if defined(FN_BROKEN_parse_errors) || defined(FN_BROKEN_network_set_parser)
  SKIP(network_set_parser);
#else
  err = network_open(NET_JSON_URL, OPEN_MODE_HTTP_GET, OPEN_TRANS_NONE);
  TEST("open JSON URL", err == FN_ERR_OK);

  /* PARSER_NONE through _XML are 0..3; anything else falls through to
   * fujicore_set_parser's default: arm. */
  err = network_set_parser(NET_JSON_URL, PARSER_XML + 1);
  TEST("set_parser with mode 4 reports an error", err != FN_ERR_OK);

  TEST_ALIVE("FujiNet still responding after an invalid parser mode");

  /* A rejected mode must not have disturbed the channel. */
  err = network_set_parser(NET_JSON_URL, PARSER_JSON);
  TEST("set_parser still works after a rejected mode", err == FN_ERR_OK);

  err = network_close(NET_JSON_URL);
  TEST("close JSON URL", err == FN_ERR_OK);
#endif /* FN_BROKEN_parse_errors */

  END_OF_TEST();
}

void test_set_parameter_errors(void)
{
  uint8_t err;

  SECTION("NET_SET_PARAMETER error paths");

#if defined(FN_BROKEN_parse_errors) || defined(FN_BROKEN_network_set_parser) \
  || defined(FN_BROKEN_network_set_query_param)
  SKIP(network_set_query_param);
#else
  network_close(NET_JSON_URL);

  /* With no parser, fujidev_set_parameter errors *before* transaction_accept,
   * so this is a NAK rather than the ERROR the rest of the file produces.
   * #1640 flags the inconsistency but leaves it alone; pin what it does. */
  err = network_set_query_param(NET_JSON_URL, 0);
  TEST("set_query_param with no parser reports an error",
               err != FN_ERR_OK);
  TEST_ALIVE("FujiNet still responding after set_query_param with no parser");

  err = network_open(NET_JSON_URL, OPEN_MODE_HTTP_GET, OPEN_TRANS_NONE);
  TEST("open JSON URL", err == FN_ERR_OK);

  /* If no parser has been set, network_set_query_param() should fail */
  err = network_set_query_param(NET_JSON_URL, 0);
  TEST("set_query_param without parser reports an error", err != FN_ERR_OK);

  err = network_set_parser(NET_JSON_URL, PARSER_JSON);
  TEST("set PARSER_JSON", err == FN_ERR_OK);

  /* 0x20 is a reserved output mode, which fujidev_set_parameter rejects. */
  err = network_set_query_param(NET_JSON_URL, 0x20);
  TEST("set_query_param with value 0x20 reports an error",
               err != FN_ERR_OK);

  err = network_set_query_param(NET_JSON_URL, 2);
  TEST("set_query_param with value 2 succeeds", err == FN_ERR_OK);

  err = network_close(NET_JSON_URL);
  TEST("close JSON URL", err == FN_ERR_OK);
#endif /* FN_BROKEN_parse_errors */

  END_OF_TEST();
}

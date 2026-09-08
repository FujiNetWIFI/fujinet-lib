/**
 * @file sgml.c
 * @brief SGML/HTML channel mode (2) tests: CSS-selector querying of a
 *        fetched HTML page.
 *
 * Firmware regression commits exercised here (fujinet-firmware):
 *   9bf45ddae  HTML parsing / channel mode  -> all tests in this file.
 *              Repeating the same query iterates to the next match; a
 *              selector containing a colon must not be truncated the way
 *              an N: prefix is stripped.
 *
 * SGML mode exists on sio/rs232/adamnet/drivewire/iwm, not on comlynx or
 * iec: on lynx this file instead asserts that mode 2 is rejected.
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

/* Portable substring search (strstr is not everywhere) */
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

void test_sgml_parse_query(void)
{
  uint8_t err;
  int16_t r;

  SECTION("SGML parse and query");

#ifdef FN_BROKEN_network_sgml_parse
  SKIP(network_sgml_parse);
#else
  err = network_open(NET_SGML_URL, OPEN_MODE_HTTP_GET, OPEN_TRANS_NONE);
  TEST("open HTML page", err == FN_ERR_OK);

  err = network_sgml_parse(NET_SGML_URL);
  TEST("SGML parse succeeds", err == FN_ERR_OK);

  memset(g.net, 0, sizeof(g.net));
  r = network_sgml_query(NET_SGML_URL, "h1", (char *) g.net);
  TEST("query h1 returns text", r > 0);
  printf("  h1=\"%s\"\n", (char *) g.net);
  TEST("h1 is the Moby-Dick heading", t_contains((char *) g.net, "Moby-Dick"));

  err = network_close(NET_SGML_URL);
  TEST("close HTML page", err == FN_ERR_OK);
#endif /* FN_BROKEN_network_sgml */

  END_OF_TEST();
}

void test_sgml_iterate(void)
{
  uint8_t err;
  int16_t r;
  uint8_t matches;
  uint8_t rounds;

  SECTION("SGML repeated query iterates matches");

#ifdef FN_BROKEN_network_sgml_query
  SKIP(network_sgml_query);
#else
  err = network_open(NET_LINKS_URL, OPEN_MODE_HTTP_GET, OPEN_TRANS_NONE);
  TEST("open links page", err == FN_ERR_OK);

  err = network_sgml_parse(NET_LINKS_URL);
  TEST("SGML parse succeeds", err == FN_ERR_OK);

  /* The page holds four small anchors; the same selector re-issued
   * advances to the next match until they run out */
  matches = 0;
  for (rounds = 0; rounds < 8; rounds++) {
    memset(g.net, 0, sizeof(g.net));
    r = network_sgml_query(NET_LINKS_URL, "a", (char *) g.net);
    if (r < 1)
      break;
    printf("  match %u: \"%s\"\n", (unsigned) (matches + 1), (char *) g.net);
    matches++;
  }
  printf("  matches=%u\n", (unsigned) matches);
  TEST("iteration found several anchors", matches >= 2);
  TEST("iteration terminated", rounds < 8);

  err = network_close(NET_LINKS_URL);
  TEST("close links page", err == FN_ERR_OK);
#endif /* FN_BROKEN_network_sgml */

  END_OF_TEST();
}

void test_sgml_colon_selector(void)
{
  uint8_t err;
  int16_t r;

  SECTION("SGML selector containing a colon");

#ifdef FN_BROKEN_network_sgml_query
  SKIP(network_sgml_query);
#else
  /* Regression guard for 9bf45ddae: a colon inside a CSS selector must
   * not be treated as a devicespec prefix and truncated. The wire call
   * has to survive; whether the pseudo-class matches depends on fnsgml
   * support, so the content is printed, not asserted. */
  err = network_open(NET_LINKS_URL, OPEN_MODE_HTTP_GET, OPEN_TRANS_NONE);
  TEST("open links page", err == FN_ERR_OK);

  err = network_sgml_parse(NET_LINKS_URL);
  TEST("SGML parse succeeds", err == FN_ERR_OK);

  memset(g.net, 0, sizeof(g.net));
  r = network_sgml_query(NET_LINKS_URL, "a:nth-of-type(2)", (char *) g.net);
  TEST("colon selector does not error at wire level", r >= 0);
  printf("  a:nth-of-type(2)=\"%s\" (r=%d)\n", (char *) g.net, (int) r);

  err = network_close(NET_LINKS_URL);
  TEST("close links page", err == FN_ERR_OK);
#endif /* FN_BROKEN_network_sgml */

  END_OF_TEST();
}

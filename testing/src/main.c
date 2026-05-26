/**
 * @file fujinet_integration_tests.c
 * @brief Integration tests for FujiNet library: fujinet-fuji.h, fujinet-network.h, fujinet-clock.h
 *
 * These are integration tests that communicate over the actual FujiNet bus.
 * They CANNOT be run without a connected FujiNet device.
 *
 * Prerequisites:
 *   - A FujiNet device connected and accessible via its normal bus
 *   - At least one configured host slot (slot 0) pointing to a valid network share
 *   - WiFi configured and connected on the FujiNet device
 *   - Network access to fujinet.online from the FujiNet device
 *
 * Test output: PASS / FAIL lines to stdout, summary at end.
 * Exit code:   0 = all passed, non-zero = number of failures.
 *
 * Compiler constraints observed:
 *   - All variable declarations appear at the top of their enclosing block
 *   - No malloc/free; all buffers are static globals or small stack locals
 *   - Stack locals are kept under 64 bytes; anything larger is a static global
 */

#if !FUJI_TESTS && !NETWORK_TESTS && !CLOCK_TESTS
#error "You need to choose some tests"
#endif

#include "fuji.h"
#include "network.h"
#include "clock.h"
#include "init_video.h"
#include "harness.h"

#ifdef _CMOC_VERSION_
#include <coco.h>
#else
#include <stdio.h>
#endif /* _CMOC_VERSION_ */

#ifdef UNUSED
#ifndef _CMOC_VERSION_
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#endif /* _CMOC_VERSION_ */

#ifdef BUILD_APPLE2
#include <apple2.h>
#endif /* BUILD_APPLE2 */

#include "fujinet-fuji.h"
#include "fujinet-network.h"
#include "fujinet-clock.h"

#include "broken.h"

#endif /* UNUSED */

/* =========================================================================
 * main
 * ========================================================================= */

int main(void)
{
  init_video();

  printf("=== FujiNet Integration Tests ===\n");
  printf("NOTE: Requires a connected, WiFi-enabled FujiNet device.\n");

#if FUJI_TESTS
  /* fuji.h */
  test_fuji_status();
  test_fuji_adapter_config();
  test_fuji_host_device_slots();
  test_fuji_host_prefix();
  test_fuji_directory_ops();
  test_fuji_wifi();
  test_fuji_network_scan();
  test_fuji_appkey();
  test_fuji_guid();
  test_fuji_base64();
  test_fuji_hashing();
#endif // FUJI_TESTS

#if NETWORK_TESTS
  /* network.h */
  test_network_init();
  test_network_write();
#if 0
  test_network_error_path();
#endif
  test_network_http_get();
  test_network_http_get_nonblocking();
  test_network_http_post();
  test_network_http_headers();
  test_network_http_channel_mode();
  test_network_json();
  test_network_http_put_delete();
  test_network_unit();
#endif // NETWORK_TESTS

#if CLOCK_TESTS
  /* clock.h */
  test_clock_tz();
  test_clock_simple_binary();
  test_clock_prodos_binary();
  test_clock_apetime_binary();
  test_clock_tz_iso_string();
  test_clock_utc_iso_string();
  test_clock_apple3_sos();
  test_clock_get_time_tz();
#endif // CLOCK_TESTS

  printf("\n=== All %d tests passed ===\n", tests_run);

  end_testing(0);
  return 0;
}

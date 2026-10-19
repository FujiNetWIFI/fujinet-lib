/**
 * @file main.c
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
 * Exit code:   0 = all passed, 1 = aborted on the first failure.
 *
 * Compiler constraints observed:
 *   - All variable declarations appear at the top of their enclosing block
 *   - No malloc/free; all buffers are static globals or small stack locals
 *   - Stack locals are kept under 64 bytes; anything larger is a static global
 */

#if !FUJI_TESTS && !NETWORK_TESTS && !NETWORK2_TESTS && !HTML_TESTS \
  && !CALMAIL_TESTS && !REGRESSION_TESTS && !CLOCK_TESTS && !DISK_TESTS \
  && !QRCODE_TESTS && !FS_TESTS && !APPKEY_TESTS && !DEVLIST_TESTS \
  && !PARSE_ERROR_TESTS && !CLIPBOARD_TESTS \
  && !NETPROTO_TESTS
#error "You need to choose some tests"
#endif

#include "broken.h"

#include "fuji.h"
#include "appkey.h"
#include "network.h"
#include "network2.h"
#include "http.h"
#include "html.h"
#include "parse_errors.h"
#include "clock.h"
#include "fdsk.h"
#include "qrcode.h"
#include "fs.h"
#include "ftp.h"
#include "devlist.h"
#include "clipboard.h"
#include "netproto.h"

#include "init_video.h"
#include "harness.h"

#ifdef _CMOC_VERSION_
#include <coco.h>
#else
#include <stdio.h>
#endif /* _CMOC_VERSION_ */

#ifdef BUILD_C64
#include "c64_find_load.h"
#endif /* BUILD_C64 */

/* =========================================================================
 * main
 * ========================================================================= */

const char *test_program = TEST_PROGRAM;

int main(void)
{
  static char line[64];

  init_video();

  printf("=== FujiNet Integration Tests ===\n");
  printf("NOTE: Requires a connected, WiFi-enabled FujiNet device.\n");

  begin_testing();

#if FUJI_TESTS
  /* fuji.h */
  test_fuji_status();
  test_fuji_adapter_config();
  test_fuji_host_device_slots();
  test_fuji_device_filename();
  test_fuji_host_prefix();
  test_fuji_directory_ops();
  test_fuji_wifi();
  test_fuji_network_scan();
  test_fuji_guid();
  test_fuji_base64();
  test_fuji_hashing();
#endif // FUJI_TESTS

#if NETPROTO_TESTS
  /* netproto.h */
  test_netproto_read_count();
  test_netproto_read_to_eof();
  test_netproto_read_partial();
#endif // NETPROTO_TESTS

#if CLIPBOARD_TESTS
  /* clipboard.h */
  test_clipboard();
  test_clipboard_refusals();
  test_clipboard_limit();
#endif // CLIPBOARD_TESTS

#if APPKEY_TESTS
  /* appkey.h */
  test_fuji_appkey();
#endif // APPKEY_TESTS

#if QR_TESTS
  test_fuji_qrcode();
#endif // QR_TESTS

#if DISK_TESTS
  /* fdsk.h */
  test_fuji_copy_file();
  test_fuji_create_new();
#endif // DISK_TESTS

#if QRCODE_TESTS
  /* qrcode.h */
  test_fuji_qrcode();
#endif // QRCODE_TESTS

#if NETWORK_TESTS
  /* network.h */
  test_network_init();
  test_network_write();
  test_network_status_after_close();
  test_network_status_after_failed_open();
  test_network_open_no_n_prefix();
  test_network_set_eol();
#if ERROR_PATH_WORKS
  test_network_error_path();
#endif
  test_multiple_network_devices();
  test_multiple_telnet_devices();
  test_network_unit();
#endif // NETWORK_TESTS

#if NETWORK2_TESTS
  /* network2.h - the v5 command additions */
  test_net2_accept();
  test_net2_short_read();
  test_net2_seek_tell();
  test_net2_seek_errors();
  test_net2_bus_read_unopened();
  test_net2_set_translation();
  test_net2_login_smoke();
  test_net2_interrupt_rate();
  test_net2_close_client();
  test_net2_udp_set_destination();
  test_net2_udp_get_remote();
  test_net2_channel_mode();
#endif // NETWORK2_TESTS

#if HTML_TESTS
  /* http.h */
  test_network_http_get();
  test_network_http_fixed_completion();
  test_network_http_chunked_completion();
  test_network_http_get_nonblocking();
  test_network_http_post();
  test_network_http_post_large();
  test_network_json_query_large();
  test_network_http_headers();
  test_network_http_channel_mode();
  test_network_json();
  test_network_http_put_plain();
  test_network_http_put_delete();
  test_network_close_failure_sticks();

  /* html.h */
  test_html_parse_query();
  test_html_iterate();
  test_html_colon_selector();
#endif // HTML_TESTS

#if PARSE_ERROR_TESTS
  /* parse_errors.h - NDevice parser error codes (fujinet-firmware #1640) */
  test_parse_no_parser();
  test_parse_after_close();
  test_parse_parser_none();
  test_json_parse_malformed();
  test_json_parse_empty_body();
  test_html_parse_empty_body();
  test_parse_error_not_sticky();
  test_query_after_failed_parse();
  test_query_no_channel();
  test_set_parser_invalid_mode();
  test_set_parameter_errors();
#endif // PARSE_ERROR_TESTS

#if FS_TESTS
  /* fs.h */
  test_fs_prefix();
  test_fs_make_test_dir();
  test_fs_create_files();
  test_fs_directory_modes();
  test_fs_long_listing();
  test_fs_read_files();
  test_fs_rename_delete();
  test_fs_dir_lifecycle();
  test_fs_directory_filter();
  test_fs_sd_hash_name();
  test_fs_lock_unlock();
  test_fs_tnfs_lock_unlock();
  test_fs_ftp_listing();
  test_ftp_crunched_name();
#endif // FS_TESTS

#if CLOCK_TESTS
  /* clock.h */
  test_clock_tz();
  test_clock_simple_binary();
  test_clock_prodos_binary();
  test_clock_apetime_binary();
  test_clock_tz_iso_string();
  test_clock_utc_iso_string();
  test_clock_apple3_sos();
  test_clock_simple_binary_with_hundredths();
  test_clock_get_time_tz();
#endif // CLOCK_TESTS

#if DEVLIST_TESTS
  /* devlist.h */
  test_fujinet_exists();
  test_clock_exists();
  test_network_exists();
#endif // DEVLIST_TESTS

  sprintf(line, "=== All %d tests passed, %d skipped ===", tests_run, tests_skipped);
  printf("\n");
  tprint(TP_ALL_PASSED, line);
  printf("\n");
  print_versions();

  end_testing(0);
  return 0;
}

#ifndef BROKEN_H
#define BROKEN_H

/* =========================================================================
 * Known non-working functions
 *
 * Define a symbol here for each function that is known to be broken.
 * Tests that call a broken function will print SKIP instead of running.
 * Remove the #define once a function is fixed and re-tested.
 *
 * Example:
 *   #define FN_BROKEN_fuji_generate_guid
 *   #define FN_BROKEN_network_http_put
 * ========================================================================= */

#include <fujinet-fuji.h>

#if FNLIB_VERSION_MAJOR < 5

#ifdef BUILD_ATARI
#define FN_BROKEN_fuji_open_directory2
#define FN_BROKEN_fuji_generate_guid
#endif /* BUILD_ATARI */

#ifdef BUILD_COCO
#define FN_BROKEN_fuji_status
#define FN_BROKEN_fuji_get_directory_position
#define FN_BROKEN_fuji_get_host_prefix
#define FN_BROKEN_fuji_base64_encode_output

#define FN_BROKEN_clock_get_time_PRODOS_BINARY
#define FN_BROKEN_clock_get_time_APETIME_BINARY
#define FN_BROKEN_clock_get_time_TZ_ISO_STRING
#define FN_BROKEN_clock_get_time_UTC_ISO_STRING
#define FN_BROKEN_clock_get_time_APPLE3_SOS_BINARY
#define FN_BROKEN_clock_get_tz
#define FN_BROKEN_clock_set_tz
#define FN_BROKEN_clock_get_time_tz
#endif /* BUILD_COCO */

#ifdef BUILD_APPLE2
#define FN_BROKEN_fuji_open_directory2
#define FN_BROKEN_fuji_get_host_prefix

#define FN_BROKEN_fuji_base64_encode_length
#define FN_BROKEN_fuji_hash_clear
#define FN_BROKEN_fuji_hash_data
#define FN_BROKEN_fuji_hash_calculate

#define FN_BROKEN_network_http_post
#endif /* BUILD_APPLE2 */

#ifdef BUILD_MSDOS
#define FN_BROKEN_fuji_get_host_slots
#define FN_BROKEN_fuji_get_host_prefix
#define FN_BROKEN_fuji_mount_host_slot
#define FN_BROKEN_fuji_get_scan_result

#define FN_BROKEN_fuji_read_appkey

#define FN_BROKEN_fuji_base64_encode_input
#define FN_BROKEN_fuji_hash_data

#define FN_BROKEN_clock_get_time_SIMPLE_BINARY
#define FN_BROKEN_clock_get_time_PRODOS_BINARY
#define FN_BROKEN_clock_get_time_APETIME_BINARY
#define FN_BROKEN_clock_get_time_TZ_ISO_STRING
#define FN_BROKEN_clock_get_time_UTC_ISO_STRING
#define FN_BROKEN_clock_get_time_APPLE3_SOS_BINARY
#define FN_BROKEN_clock_get_tz
#define FN_BROKEN_clock_set_tz
#define FN_BROKEN_clock_get_time_tz

#define FN_BROKEN_network_write
#define FN_BROKEN_network_read
#define FN_BROKEN_network_http_post
#define FN_BROKEN_network_http_start_add_headers
#define FN_BROKEN_network_http_set_channel_mode
#define FN_BROKEN_network_json_parse
#define FN_BROKEN_network_http_put

#endif /* BUILD_MSDOS */

#ifdef BUILD_ADAM
#define FN_BROKEN_fuji_get_host_slots
#define FN_BROKEN_fuji_put_host_slots

#define FN_BROKEN_network_close
#define FN_BROKEN_network_http_add_header
#define FN_BROKEN_network_http_delete
#define FN_BROKEN_network_http_end_add_headers
#define FN_BROKEN_network_http_post
#define FN_BROKEN_network_http_post_bin
#define FN_BROKEN_network_http_put
#define FN_BROKEN_network_http_set_channel_mode
#define FN_BROKEN_network_http_start_add_headers
#define FN_BROKEN_network_init
#define FN_BROKEN_network_json_parse
#define FN_BROKEN_network_json_query
#define FN_BROKEN_network_open
#define FN_BROKEN_network_read
#define FN_BROKEN_network_read_nb
#define FN_BROKEN_network_status
#define FN_BROKEN_network_unit
#define FN_BROKEN_network_write

#define FN_BROKEN_clock_get_time_SIMPLE_BINARY
#define FN_BROKEN_clock_get_time_PRODOS_BINARY
#define FN_BROKEN_clock_get_time_APETIME_BINARY
#define FN_BROKEN_clock_get_time_TZ_ISO_STRING
#define FN_BROKEN_clock_get_time_UTC_ISO_STRING
#define FN_BROKEN_clock_get_time_APPLE3_SOS_BINARY
#define FN_BROKEN_clock_get_time_tz
#define FN_BROKEN_clock_get_tz
#define FN_BROKEN_clock_set_tz
#endif /* BUILD_ADAM */

#else /* FNLIB_VERSION_MAJOR >= 5 */

#ifdef BUILD_MSDOS
// Missing from firmware
#define FN_BROKEN_fuji_base64_encode_input
#define FN_BROKEN_fuji_hash_data
#endif /* BUILD_MSDOS */

#ifdef BUILD_APPLE2
// Missing from firmware
#define FN_BROKEN_fuji_base64_encode_input
#define FN_BROKEN_fuji_hash_data
#endif /* BUILD_APPLE2 */

#endif /* FNLIB_VERSION_MAJOR */

#endif /* BROKEN_H */

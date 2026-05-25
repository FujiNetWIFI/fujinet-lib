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

#ifdef BUILD_ATARI
#define FN_BROKEN_fuji_open_directory2
//#define FN_BROKEN_fuji_generate_guid
#define FN_BROKEN_fuji_get_host_prefix
#define FN_BROKEN_fuji_base64_encode_input
#define FN_BROKEN_fuji_hash_data

#define FN_BROKEN_network_http_put
#endif // BUILD_ATARI

#ifdef BUILD_COCO
#define FN_BROKEN_fuji_status
#define FN_BROKEN_fuji_set_host_prefix
#define FN_BROKEN_fuji_base64_encode_output
#define FN_BROKEN_fuji_hash_size
#define FN_BROKEN_fuji_base64_encode_length
#define FN_BROKEN_fuji_get_directory_position

#define FN_BROKEN_clock_get_time_tz
#define FN_BROKEN_clock_get_tz
#define FN_BROKEN_clock_set_tz
#endif // BUILD_COCO

#ifdef BUILD_APPLE2
#define FN_BROKEN_fuji_open_directory2
#define FN_BROKEN_fuji_get_device_slots
#define FN_BROKEN_fuji_get_host_prefix
#define FN_BROKEN_fuji_base64_encode_input

#define FN_BROKEN_fuji_hash_add
#define FN_BROKEN_fuji_hash_calculate
#define FN_BROKEN_fuji_hash_clear
#define FN_BROKEN_fuji_hash_data
#define FN_BROKEN_fuji_hash_size
#endif

#ifdef BUILD_C64
#define FN_BROKEN_clock_get_time
#define FN_BROKEN_clock_get_time_tz
#define FN_BROKEN_clock_get_tz
#define FN_BROKEN_clock_set_tz
#endif

#ifdef BUILD_MSDOS
#define FN_BROKEN_clock_get_time
#define FN_BROKEN_clock_get_time_tz
#define FN_BROKEN_clock_get_tz
#define FN_BROKEN_clock_set_tz
#endif

#endif /* BROKEN_H */

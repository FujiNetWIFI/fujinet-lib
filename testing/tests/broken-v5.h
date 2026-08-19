#if FNLIB_VERSION_MAJOR >= 5

#ifdef BUILD_APPLE2
// Missing from firmware
#define FN_BROKEN_network_set_eol
#endif /* BUILD_APPLE2 */

#if defined(BUILD_ADAM) || defined(BUILD_ADAM_CPM)
// Missing from firmware
#define FN_BROKEN_fuji_status
#define FN_BROKEN_fuji_get_host_prefix
#define FN_BROKEN_fuji_get_wifi_enabled
#define FN_BROKEN_fuji_scan_for_networks
#define FN_BROKEN_network_set_eol
#define FN_BROKEN_clock_get_time_UTC_ISO_STRING
#endif /* BUILD_ADAM || BUILD_ADAM_CPM */

#ifdef BUILD_COCO
// Missing from firmware
#define FN_BROKEN_network_set_eol
#endif /* BUILD_COCO */

#endif // FNLIB_VERSION_MAJOR >= 5

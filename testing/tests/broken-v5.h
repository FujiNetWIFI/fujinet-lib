#if FNLIB_VERSION_MAJOR >= 5

#ifdef BUILD_APPLE2
// Missing from firmware
#define FN_BROKEN_network_set_eol
#endif /* BUILD_APPLE2 */

#if defined(BUILD_ADAM) || defined(BUILD_ADAM_CPM)
// Missing from firmware
#define FN_BROKEN_network_set_eol

// adamnet has no clock device; only the Fuji device's GET TIME exists
#define FN_BROKEN_clock_get_time_APETIME_BINARY
#define FN_BROKEN_clock_get_time_APPLE3_SOS_BINARY
#define FN_BROKEN_clock_get_time_PRODOS_BINARY
#define FN_BROKEN_clock_get_time_SIMPLE_BINARY
#define FN_BROKEN_clock_get_time_SIMPLE_BINARY_WITH_HUNDREDTHS
#define FN_BROKEN_clock_get_time_tz
#define FN_BROKEN_clock_get_time_TZ_ISO_STRING
#define FN_BROKEN_clock_get_time_UTC_ISO_STRING
#define FN_BROKEN_clock_get_tz
#define FN_BROKEN_clock_set_tz
#endif /* BUILD_ADAM || BUILD_ADAM_CPM */

#ifdef BUILD_COCO
// Missing from firmware
#define FN_BROKEN_network_set_eol
#endif /* BUILD_COCO */

#endif // FNLIB_VERSION_MAJOR >= 5

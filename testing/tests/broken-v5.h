#if FNLIB_VERSION_MAJOR >= 5

#ifdef BUILD_ADAM
#define FN_BROKEN_fuji_mount_host_slot_error
#define FN_BROKEN_network_open_invalid
#endif /* BUILD_ADAM */

#ifdef BUILD_C64
#define FN_BROKEN_network_open_invalid
#endif /* BUILD_C64 */

#endif // FNLIB_VERSION_MAJOR >= 5

#if FNLIB_VERSION_MAJOR >= 5

/* The second client needs N3; the Adam has only N1 and N2 */
#ifdef BUILD_ADAM
#define FN_BROKEN_network_accept_second_client
#endif /* BUILD_ADAM */

/* IEC's systemBus::transaction_error() is still a no-op
 * ("// FIXME - signal error somehow", fujinet-firmware lib/bus/iec/iec.cpp),
 * so a command the device rejects still comes back FN_ERR_OK here. The
 * STATUS error byte is the only channel that carries the failure. */
#ifdef BUILD_C64
#define FN_BROKEN_network_open_invalid
#define FN_BROKEN_transaction_error
#endif /* BUILD_C64 */

/* The MSX bus has no fuji_bus_appkey_read/write */
#ifdef BUILD_MSX
#define FN_BROKEN_fuji_set_appkey_details
#define FN_BROKEN_fuji_read_appkey
#define FN_BROKEN_fuji_write_appkey
#endif /* BUILD_MSX */

#endif // FNLIB_VERSION_MAJOR >= 5

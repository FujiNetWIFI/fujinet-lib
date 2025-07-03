/*
 * device specific error value, e.g. SmartPort specific errors
 */
unsigned char fn_device_error;


// network status values, these come from FujiNet itself, not the host machine.

/*
 * general network error
 */
unsigned char fn_network_error;

/*
 * bytes waiting
 */
unsigned short fn_network_bw;

/*
 * connection status
 */
unsigned char fn_network_conn;

/*
 * count of bytes read in last network_read call, so applications can nul terminate as needed.
 */
unsigned short fn_bytes_read;

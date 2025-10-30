#ifndef _CMOC_VERSION_
#include <stdint.h>
#else
#include <cmoc.h>
#include <coco.h>
#endif

/*
 * device specific error value, e.g. SmartPort specific errors
 */
uint8_t fn_device_error;

/*
 * default timeout for fuji/network commands where supported
 */

uint8_t fn_default_timeout = 15;


// network status values, these come from FujiNet itself, not the host machine.

/*
 * general network error
 */
uint8_t fn_network_error;

/*
 * bytes waiting
 */
uint16_t fn_network_bw;

/*
 * connection status
 */
uint8_t fn_network_conn;

/*
 * count of bytes read in last network_read call, so applications can nul terminate as needed.
 */
uint16_t fn_bytes_read;

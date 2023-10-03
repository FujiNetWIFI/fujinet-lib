/**
 * @brief FujiNet Network Device Library
 * @license gpl v. 3, see LICENSE for details.
 */

#ifndef FUJINET_NETWORK_H
#define FUJINET_NETWORK_H

#include <stdint.h>

/**
 * @brief  Get Network Device Status byte 
 * @param  devicespec pointer to device specification of form: N:PROTO://[HOSTNAME]:PORT/PATH/.../
 * @param  bw pointer to where to put bytes waiting
 * @param  c pointer to where to put connection status
 * @param  err to where to put network error byte.
 * @return fujinet-network status/error code (See FN_ERR_* values)
 */
uint8_t network_status(char *devicespec, uint16_t *bw, uint8_t *c, uint8_t *err);

/**
 * @brief  Close Connection
 * @param  devicespec pointer to device specification of form: N:PROTO://[HOSTNAME]:PORT/PATH/.../
 * @return fujinet-network error code (See FN_ERR_* values)
 */
uint8_t network_close(char* devicespec);

/**
 * @brief  Open Connection
 * @param  devicespec pointer to device specification of form: N:PROTO://[HOSTNAME]:PORT/PATH/.../
 * @param  mode (4=read, 8=write, 12=read/write, 13=POST, etc.)
 * @param  trans translation mode (CR/LF to other line endings)
 * @return fujinet-network error code (See FN_ERR_* values)
 */
uint8_t network_open(char* devicespec, uint8_t mode, uint8_t trans);

/**
 * @brief  Read from channel
 * @param  devicespec pointer to device specification, e.g. "N1:HTTPS://fujinet.online/"
 * @param  buf Buffer
 * @param  len length
 * @return fujinet-network error code (See FN_ERR_* values)
 */
uint8_t network_read(char* devicespec, uint8_t *buf, uint16_t len);

/**
 * @brief  Write to network 
 * @param  devicespec pointer to device specification, e.g. "N1:HTTPS://fujinet.online/"
 * @param  buf Buffer
 * @param  len length
 * @return fujinet-network error code (See FN_ERR_* values)
 */
uint8_t network_write(char* devicespec, uint8_t *buf, uint16_t len);

/**
 * @brief  Device specific direct control commands
 * @param  cmd Command byte to send
 * @param  aux1 Auxiliary byte 1
 * @param  aux2 Auxiliary byte 2
 * @param  devicespec pointer to device specification, e.g. "N1:HTTPS://fujinet.online/"
 * @param  ... varargs - Device specific additional parameters to pass to the network device
 * @return fujinet-network error code (See FN_ERR_* values)
 */
uint8_t network_ioctl(uint8_t cmd, uint8_t aux1, uint8_t aux2, char* devicespec, ...);

/**
 * @brief  Parse the currently open JSON location
 * @param  devicespec pointer to device specification, e.g. "N1:HTTPS://fujinet.online/"
 * @param  mode (4=read, 8=write, 12=read/write, 13=POST, etc.)
 * @return fujinet-network error code (See FN_ERR_* values)
 * 
 * This will set the channel mode to JSON, which will be unset in the close.
 */
uint8_t network_json_parse(char *devicespec, uint8_t mode);

/**
 * @brief  Perform JSON query
 * @param  devicespec pointer to device specification, e.g. "N1:HTTPS://fujinet.online/"
 * @param  query pointer to string containing json path to query
 * @param  s pointer to receiving string, nul terminated, if no data was retrieved, returns empty string
 * @return fujinet-network error code (See FN_ERR_* values)
 * 
 * Assumes an open and parsed json.
 */
uint8_t network_json_query(char *devicespec, char *query, char *s);


#define FN_ERR_OK               (0x00)      /* No error */
#define FN_ERR_IO_ERROR         (0x01)      /* There was IO error/issue with the device */
#define FN_ERR_BAD_CMD          (0x02)      /* Function called with bad arguments */
#define FN_ERR_OFFLINE          (0x03)      /* The device is offline */
#define FN_ERR_WARNING          (0x04)      /* Device specific non-fatal warning issued */

#define FN_ERR_UNKNOWN          (0xff)      /* Device specific error we didn't handle */

#endif /* FUJINET_NETWORK_H */
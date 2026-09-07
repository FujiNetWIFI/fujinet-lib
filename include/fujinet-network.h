/**
 * @brief FujiNet Network Device Library
 * @license gpl v. 3, see LICENSE for details.
 */

#ifndef FUJINET_NETWORK_H
#define FUJINET_NETWORK_H

#include <fujinet-int.h>
#include <fujinet-err.h>
#include <fujinet-bus.h>
#include <fujinet-commands.h>
#include <fujinet-const.h>

#ifdef __CBM__

// For DATA transfers, we want to undo some of the charmap settings in CC65.
// So map the characters in the range 5b-60, 7b-7f back to themselves, as we want to send them as-is to FujiNet.
// This undoes some "look alike" character that move into 0x80+ range, which in FN are converted to 3 byte utf-8 chars incorrectly.

#pragma charmap (0x5B, 0x5B)
#pragma charmap (0x5C, 0x5C)
#pragma charmap (0x5D, 0x5D)
#pragma charmap (0x5E, 0x5E)
#pragma charmap (0x5F, 0x5F)
#pragma charmap (0x60, 0x60)
#pragma charmap (0x7B, 0x7B)
#pragma charmap (0x7C, 0x7C)
#pragma charmap (0x7D, 0x7D)
#pragma charmap (0x7E, 0x7E)
#pragma charmap (0x7F, 0x7F)

#endif

#define MAX_JSON_QUERY_LEN 256

enum {
  PARSER_NONE = 0,
  PARSER_JSON = 1,
  PARSER_SGML = 2,
};

/**
 * The number of bytes read in the last call to network_read().
 * This can be less than the amount asked for, if there aren't enough bytes available from target.
 * This allows applications to add nul terminators etc.
 */
extern uint16_t fn_bytes_read;

/**
 * Convert device specific error in code to FujiNet Network library error, agnostic of device.
 * Library code calls this when it encounters an error to return value applications should use.
 */
uint8_t fn_error(uint8_t code);

/*
 * The status returned by the most recent device status poll (from
 * network_status or the poll inside network_read/network_read_nb).
 * avail   : bytes waiting
 * status  : connected status, 1 while still reading a resource, 0 when
 *           the current read has completed
 * errcode : the error status returned from FN, e.g. 136 for EOF, 1
 *           for normal OK status (don't ask why)
 */
typedef struct {
  uint16_t avail;
  uint8_t status;
  uint8_t errcode;
} NetworkStatus;

extern NetworkStatus nw_status;

/*
 * Convenience aliases for the nw_status fields, set during network_read.
 * You can capture your own using network_status.
 */
#define fn_network_bw    (nw_status.avail)
#define fn_network_conn  (nw_status.status)
#define fn_network_error (nw_status.errcode)

/**
 * @brief  Initialise network device
 * Allows initialisation of network to perform any platform dependent checks, and allow applications to
 * exit early if there is a network issue.
 * @return fujinet-network status/error code (See FN_ERR_* values) and set device specific error if there is any
 */
FN_ERR network_init(void);

/**
 * @brief  Get Network Device Status byte
 * @param  devicespec pointer to device specification of form: N:PROTO://[HOSTNAME]:PORT/PATH/.../
 * @param  bw pointer to where to put bytes waiting
 * @param  c pointer to where to put connection status
 * @param  err to where to put network error byte.
 * @return fujinet-network status/error code (See FN_ERR_* values)
 */
FN_ERR network_status(const char *devicespec, uint16_t *bw, uint8_t *c, uint8_t *err);

/**
 * @brief  Close Connection
 * @param  devicespec pointer to device specification of form: N:PROTO://[HOSTNAME]:PORT/PATH/.../
 * @return fujinet-network error code (See FN_ERR_* values)
 */
FN_ERR network_close(const char* devicespec);

/**
 * @brief  Open Connection
 * @param  devicespec pointer to device specification of form: N:PROTO://[HOSTNAME]:PORT/PATH/.../
 * @param  mode (4=read, 8=write, 12=read/write, 13=POST, etc.)
 * @param  trans translation mode (CR/LF to other line endings; 0=none, 1=CR, 2=LF, 3=CRLF, 4=Pet)
 * @return fujinet-network error code (See FN_ERR_* values)
 */
FN_ERR network_open(const char* devicespec, uint8_t mode, uint8_t trans);
#if FUJI_VARIABLE_LEN_PACKETS
#define NETWORK_OPEN_LEN strlen(devicespec)
#else
#define NETWORK_OPEN_LEN MAX_FILENAME_LEN
#endif /* FUJI_VARIABLE_LEN_PACKETS */

/**
 * @brief  Non-blocking read from channel
 *
 * The read will grab whatever is waiting in the FujiNet buffer. If fewer than the requested len, the return count will reflect this.
 * Errors are returned as the negative value of the FUJI standard error. fn_network_error contains the device specific error code. fn_bytes_read will be 0 on errors.
 *
 * @param  devicespec pointer to device specification, e.g. "N1:HTTPS://fujinet.online/"
 * @param  buf Buffer
 * @param  len length
 * @return Bytes read, or negative value of fujinet-network error code (See FN_ERR_* values) with fn_network_error containing real error code
 */
#if defined(__ADAM__) || defined(__COLECOADAM__)
extern int16_t network_read_nb_adam(const char *devicespec, void *buf, uint16_t len);
#define network_read_nb(devspec, buf, len) network_read_nb_adam(devspec, buf, len)
#else /* ! (__ADAM__ || __COLECOADAM__) */
int16_t network_read_nb(const char* devicespec, void *buf, uint16_t len);
#endif /* __ADAM__ || __COLECOADAM__ */

/**
 * @brief  Read from channel
 *
 * The read will block until it has read all the bytes requested from the device, or the EOF is hit.
 * This will block waiting for as much data as it can, so that the client does not need to handle counting.
 * Errors are returned as the negative value of the error. fn_network_error contains the device specific error code. fn_bytes_read will contain the count of bytes read before error occurred.
 *
 * @param  devicespec pointer to device specification, e.g. "N1:HTTPS://fujinet.online/"
 * @param  buf Buffer
 * @param  len length
 * @return Bytes read, or negative value of fujinet-network error code (See FN_ERR_* values) with fn_network_error containing real error code
 */
int16_t network_read(const char* devicespec, void *buf, uint16_t len);

/**
 * @brief  Write to network
 * @param  devicespec pointer to device specification, e.g. "N1:HTTPS://fujinet.online/"
 * @param  buf Buffer
 * @param  len length
 * @return fujinet-network error code (See FN_ERR_* values)
 */
FN_ERR network_write(const char* devicespec, const void *buf, uint16_t len);

/**
 * @brief  Parse the currently open JSON location
 * @param  devicespec pointer to device specification, e.g. "N1:HTTPS://fujinet.online/"
 * @return fujinet-network error code (See FN_ERR_* values)
 *
 * This will set the parser to JSON, which will be unset in the close.
 */
FN_ERR network_json_parse(const char *devicespec);

/**
 * @brief  Perform JSON query
 * @param  devicespec pointer to device specification, e.g. "N1:HTTPS://fujinet.online/"
 * @param  query pointer to string containing json path to query, e.g. "/path/field". No need to add device drive.
 * @param  buffer pointer to receiving string, nul terminated, if no data was retrieved, sets it to an empty string
 * @return Bytes read, or negative values represent fujinet-network error code (See FN_ERR_* values)
 *
 * Assumes an open and parsed json.
 */
#if defined(__ADAM__) || defined(__COLECOADAM__)
extern int16_t network_json_query_adam(const char *devicespec, const char *query, char *buffer);
#define network_json_query(devspec, query, buffer) network_json_query_adam(devspec, query, buffer)
#else /* ! (__ADAM__ || __COLECOADAM__) */
int16_t network_json_query(const char *devicespec, const char *query, char *buffer);
#endif /* __ADAM__ || __COLECOADAM__ */

/**
 * @brief  Sets the parser.
 * @param  devicespec pointer to device specification, e.g. "N1:HTTPS://fujinet.online/"
 * @param  mode The mode to set
 * @return fujinet-network error code (See FN_ERR_* values)
 *
 * Assumes an open connection.
 */
#define network_http_set_mode(devicespec, mode) (NETCALL_A1_A2(NETCMD_SET_HTTP_MODE, network_unit(devicespec), 0, mode) ? FN_ERR_OK : FN_ERR_IO_ERROR)
// Legacy function name
#define network_http_set_channel_mode network_http_set_mode

/**
 * @brief  Start adding headers.
 * @param  devicespec pointer to device specification, e.g. "N1:HTTPS://fujinet.online/"
 * @return fujinet-network error code (See FN_ERR_* values)
 *
 * Assumes an open connection. After calling this, add any headers with network_http_add_header, and finally call network_http_end_add_headers
 */
#define network_http_start_add_headers(devicespec) network_http_set_mode(devicespec, HTTP_CHAN_MODE_SET_HEADERS)

/**
 * @brief  End adding headers.
 * @param  devicespec pointer to device specification, e.g. "N1:HTTPS://fujinet.online/"
 * @return fujinet-network error code (See FN_ERR_* values)
 *
 * Assumes an open connection. Completes header adding, and sets mode back to BODY
 */
#define network_http_end_add_headers(devicespec) network_http_set_mode(devicespec, HTTP_CHAN_MODE_BODY)

/**
 * @brief  Add header to HTTP request
 * @param  devicespec pointer to device specification, e.g. "N1:HTTPS://fujinet.online/"
 * @param  header pointer to string containing full header to add, e.g. "Accept: application/json"
 * @return fujinet-network error code (See FN_ERR_* values)
 *
 * Assumes an open connection.
 */
#define network_http_add_header(devicespec, header) network_write(devicespec, header, strlen(header))


/**
 * @brief  Send POST HTTP request - assumes data is a string with nul terminator. This will not be able to send the 00 byte
 * @param  devicespec pointer to device specification, e.g. "N1:HTTPS://fujinet.online/"
 * @param  data text data to post
 * @return fujinet-network error code (See FN_ERR_* values)
 *
 * Assumes an open connection.
 */
FN_ERR network_http_post(const char *devicespec, const char *data);


/**
 * @brief  Send POST HTTP request, sends binary data from data location for length len, which allows sending arbitrary binary data
 * @param  devicespec pointer to device specification, e.g. "N1:HTTPS://fujinet.online/"
 * @param  data binary data to post
 * @param  len length of binary data to send
 * @return fujinet-network error code (See FN_ERR_* values)
 *
 * Assumes an open connection.
 */
FN_ERR network_http_post_bin(const char *devicespec, const uint8_t *data, uint16_t len);

/**
 * @brief  Send PUT HTTP request
 * @param  devicespec pointer to device specification, e.g. "N1:HTTPS://fujinet.online/"
 * @param  data data to put
 * @return fujinet-network error code (See FN_ERR_* values)
 *
 * Assumes an open connection.
 */
#define network_http_put(devicespec, data) network_http_post(devicespec, data)

/**
 * @brief  Send DELETE HTTP request
 * @param  devicespec pointer to device specification, e.g. "N1:HTTPS://fujinet.online/"
 * @param  trans translation value
 * @return fujinet-network error code (See FN_ERR_* values)
 *
 * This will open a connection, consumer can then query the data, and must close the connection.
 */
#define network_http_delete(devicespec, trans) network_open(devicespec, OPEN_MODE_HTTP_DELETE_H, trans)

/**
 * @brief  Internal routine to get the network UNIT id from the devicespec, i.e. Nx: find the "x" value
 * @param  devicespec pointer to device specification, e.g. "N1:HTTPS://fujinet.online/"
 * @return unit number
 *
 */
uint8_t network_unit(const char *devicespec);

/**
 * @brief Delete file from FS endpoint (e.g. TNFS, FTP, HTTPS, SMB)
 * @param devicespec Pointer to device specification e.g. "N1:TNFS://TMA-2/foo.txt"
 * @return fujinet-network error code (see FN_ERR_* values)
 */
#if FUJI_VARIABLE_LEN_PACKETS
#include <string.h>
#define NETWORK_FS_LEN(devicespec) strlen(devicespec)
#else
#define NETWORK_FS_LEN(devicespec) MAX_FILENAME_LEN
#endif /* FUJI_VARIABLE_LEN_PACKETS */

#define network_fs_command(cmd, devicespec)                 \
  (NETCALL_A1_A2_D(cmd, network_unit(devicespec), 0, 0, devicespec,       \
             NETWORK_FS_LEN(devicespec)) ? FN_ERR_OK : FN_ERR_IO_ERROR)

#define network_fs_delete(devicespec) network_fs_command(NETCMD_DELETE, devicespec)

/**
 * @brief Rename file on FS endpoint (e.g. TNFS, FTP, HTTPS, SMB)
 * @param devicespec Pointer to device specification, with new name after comma, e.g. "N1:TNFS://TMA-2/foo.txt,bar.txt"
 * @return fujinet-network error code (see FN_ERR_* values)
 */
#define network_fs_rename(devicespec) network_fs_command(NETCMD_RENAME, devicespec)

/**
 * @brief Lock file (make read only) on FS (e.g. TNFS, FTP, HTTPS, SMB)
 * @param devicespec Pointer to device specification "N1:TNFS://TMA-2/foo.txt"
 * @return fujinet-network error code (see FN_ERR_* values)
 */
#define network_fs_lock(devicespec) network_fs_command(NETCMD_LOCK, devicespec)

/**
 * @brief Unlock file (make read/write) on FS (e.g. TNFS, FTP, HTTPS, SMB)
 * @param devicespec Pointer to device specification "N1:TNFS://TMA-2/foo.txt"
 * @return fujinet-network error code (see FN_ERR_* values)
 */
#define network_fs_unlock(devicespec) network_fs_command(NETCMD_UNLOCK, devicespec)

/**
 * @brief Make directory on FS (e.g. TNFS, FTP, HTTPS, SMB)
 * @param devicespec pointer to devicespec "N1:TNFS://TMA-2/newdir"
 * @return fujinet-network error code (see FN_ERR_* values)
 */
#define network_fs_mkdir(devicespec) network_fs_command(NETCMD_MKDIR, devicespec)

/**
 * @brief Remove directory on FS (e.g. TNFS, FTP, HTTPS, SMB)
 * @param devicespec pointer to devicespec "N1:TNFS://TMA-2/newdir"
 * @return fujinet-network error code (see FN_ERR_* values)
 * @verbose Directory must be empty!
 */
#define network_fs_rmdir(devicespec) network_fs_command(NETCMD_RMDIR, devicespec)

/**
 * @brief Change directory on FS (e.g. TNFS, FTP, HTTPS, SMB)
 * @param devicespec Pointer to devicespec "N1:TNFS://TMA-2/dir"
 * @return fujinet-network error code (see FN_ERR_* values)
 */
#define network_fs_cd(devicespec)                                       \
  (NETCALL_D(NETCMD_CHDIR, network_unit(devicespec), devicespec, NETWORK_FS_LEN(devicespec)) \
   ? FN_ERR_OK : FN_ERR_IO_ERROR)

#define network_fs_pwd(devicespec, cwd)                                 \
  (NETCALL_RV(NETCMD_GETCWD, network_unit(devicespec), cwd, MAX_FILENAME_LEN) \
   ? FN_ERR_OK : FN_ERR_IO_ERROR)

FN_ERR network_accept(const char* devicespec);
extern size_t network_json_strip_newlines(char *buffer, size_t buflen);

/* Returns true if the network has data/error ready */
extern bool network_proceed(void);
/* Returns true if the hardware supports checking a proceed pin */
extern bool network_has_proceed(void);
/* Set the native end-of-line string for use with translation */
extern FN_ERR network_set_eol(const char *devicespec, const char *eol);

/**
 * @brief  Seek to an absolute byte position in the open channel (XIO 37 POINT)
 * @param  devicespec pointer to device specification, e.g. "N1:TNFS://host/file.bin"
 * @param  pos absolute byte position (24-bit on Atari SIO, 32-bit elsewhere)
 * @return fujinet-network error code (See FN_ERR_* values)
 *
 * Only supported when parser is NONE, and only by protocols that can
 * seek (TNFS/SD/SMB/NFS read+write; HTTP(S) read only via Range requests).
 */
FN_ERR network_seek(const char *devicespec, uint32_t pos);

/**
 * @brief  Report the current byte position in the open channel (XIO 38 NOTE)
 * @param  devicespec pointer to device specification, e.g. "N1:TNFS://host/file.bin"
 * @param  pos pointer to where to put the position
 * @return fujinet-network error code (See FN_ERR_* values)
 */
FN_ERR network_tell(const char *devicespec, uint32_t *pos);

/**
 * @brief  Set the translation mode applied to subsequent opens
 * @param  devicespec pointer to device specification, e.g. "N1:"
 * @param  trans translation mode (0=none, 1=CR, 2=LF, 3=CRLF; 0xFF makes opens ignore their aux2)
 * @return fujinet-network error code (See FN_ERR_* values)
 *
 * On SIO the value is sticky and ORed into the aux2 of subsequent opens,
 * except for directory opens where aux2 is a format code.
 */
FN_ERR network_set_translation(const char *devicespec, uint8_t trans);

/**
 * @brief  Set the username used to authenticate the next open (e.g. SMB, FTP)
 * @param  devicespec pointer to device specification, e.g. "N1:"
 * @param  username bare username, no Nn: prefix
 * @return fujinet-network error code (See FN_ERR_* values)
 *
 * Call before network_open. On fixed-length-packet platforms a full 256 byte
 * frame is sent, so up to 256 bytes past the string must be readable.
 */
FN_ERR network_set_username(const char *devicespec, const char *username);

/**
 * @brief  Set the password used to authenticate the next open (e.g. SMB, FTP)
 * @param  devicespec pointer to device specification, e.g. "N1:"
 * @param  password bare password, no Nn: prefix
 * @return fujinet-network error code (See FN_ERR_* values)
 */
FN_ERR network_set_password(const char *devicespec, const char *password);

/**
 * @brief  Set the status-poll interrupt rate (SIO PROCEED timer)
 * @param  devicespec pointer to device specification, e.g. "N1:"
 * @param  rate rate in milliseconds
 * @return fujinet-network error code (See FN_ERR_* values)
 */
FN_ERR network_set_timer_rate(const char *devicespec, uint8_t rate);

/**
 * @brief  Close an accepted TCP client connection, keeping the server listening
 * @param  devicespec pointer to device specification, e.g. "N1:TCP://:6502/"
 * @return fujinet-network error code (See FN_ERR_* values)
 *
 * Companion to network_accept.
 */
FN_ERR network_close_client(const char *devicespec);

/**
 * @brief  Set the destination for an open UDP channel
 * @param  dest_spec destination of the form "N1:host:port"
 * @return fujinet-network error code (See FN_ERR_* values)
 */
FN_ERR network_udp_set_destination(const char *dest_spec);

/**
 * @brief  Get the remote "ip:port" a UDP datagram was last received from
 * @param  devicespec pointer to device specification, e.g. "N1:UDP://:5000/"
 * @param  buf receiving buffer, nul terminated on return (256 bytes on fixed-length platforms)
 * @param  len buffer length
 * @return fujinet-network error code (See FN_ERR_* values)
 *
 * Only implemented on FujiNet-PC builds; ESP32 firmware rejects it.
 */
FN_ERR network_udp_get_remote(const char *devicespec, char *buf, uint16_t len);

/**
 * @brief  Set a JSON processing parameter (Atari SIO only)
 * @param  devicespec pointer to device specification, e.g. "N1:"
 * @param  param 0 = query flags (values > 2 rejected), 1 = query line-ending character
 * @param  value the value to set
 * @return fujinet-network error code (See FN_ERR_* values)
 */
FN_ERR network_json_set_parameters(const char *devicespec, uint8_t param, uint8_t value);
#define network_json_set_query_param(devicespec, flags) network_json_set_parameters(devicespec, 0, flags)
#define network_json_set_line_ending(devicespec, ch) network_json_set_parameters(devicespec, 1, ch)

/**
 * @brief  Ask which direction a network command transfers data (Atari SIO only)
 * @param  devicespec pointer to device specification, e.g. "N1:"
 * @param  cmd the command byte to query
 * @param  dstats pointer to where to put the reply: 0x00 none, 0x40 read, 0x80 write, 0xFF invalid
 * @return fujinet-network error code (See FN_ERR_* values)
 *
 * Replaces the old SIO special-command inquiry mechanism.
 */
FN_ERR network_get_dstats(const char *devicespec, uint8_t cmd, uint8_t *dstats);

/**
 * @brief  Set the parser: PARSER_NONE, _JSON or _SGML
 * @param  devicespec pointer to device specification, e.g. "N1:HTTPS://fujinet.online/"
 * @param  parser to set
 * @return fujinet-network error code (See FN_ERR_* values)
 *
 * Assumes an open connection. network_json_parse and network_sgml_parse
 * set the mode themselves.
 */
FN_ERR network_set_parser(const char *devicespec, uint8_t mode);

/**
 * @brief  Parse the currently open channel as SGML/HTML/XML
 * @param  devicespec pointer to device specification, e.g. "N1:HTTPS://fujinet.online/"
 * @return fujinet-network error code (See FN_ERR_* values)
 *
 * Sets the parser to SGML. Query with network_sgml_query using a CSS
 * selector; repeating the same query advances to the next match.
 */
FN_ERR network_sgml_parse(const char *devicespec);

/**
 * @brief  Perform SGML query with a CSS selector, e.g. "div.content a"
 *
 * Same wire command as network_json_query; assumes network_sgml_parse was called.
 */
#define network_sgml_query(devicespec, selector, buffer) network_json_query(devicespec, selector, buffer)

#define network_unit_status(unit, nws) (!NETCALL_RV(FUJICMD_STATUS, unit, nws, sizeof(NetworkStatus)))

#define OPEN_MODE_READ          (0x04)
#define OPEN_MODE_WRITE         (0x08)
#define OPEN_MODE_RW            (0x0C)

#define OPEN_MODE_HTTP_GET      (0x04)
#define OPEN_MODE_HTTP_PUT      (0x08)
#define OPEN_MODE_HTTP_GET_H    (0x0C)
#define OPEN_MODE_HTTP_POST     (0x0D)
#define OPEN_MODE_HTTP_PUT_H    (0x0E)
#define OPEN_MODE_HTTP_DELETE   (0x05)
#define OPEN_MODE_HTTP_DELETE_H (0x09)

#define OPEN_TRANS_NONE         (0x00)
#define OPEN_TRANS_CR           (0x01)
#define OPEN_TRANS_LF           (0x02)
#define OPEN_TRANS_CRLF         (0x03)
#define OPEN_TRANS_PET          (0x04)

#define HTTP_CHAN_MODE_BODY             (0)
#define HTTP_CHAN_MODE_COLLECT_HEADERS  (1)
#define HTTP_CHAN_MODE_GET_HEADERS      (2)
#define HTTP_CHAN_MODE_SET_HEADERS      (3)
#define HTTP_CHAN_MODE_POST_SET_DATA    (4)

#endif /* FUJINET_NETWORK_H */

#ifndef FUJINET_ERR_H
#define FUJINET_ERR_H

#include <fujinet-int.h>

#define FN_ERR_OK               (0x00)      /* No error */
#define FN_ERR_IO_ERROR         (0x01)      /* There was IO error/issue with the device */
#define FN_ERR_BAD_CMD          (0x02)      /* Function called with bad arguments */
#define FN_ERR_OFFLINE          (0x03)      /* The device is offline */
#define FN_ERR_WARNING          (0x04)      /* Device specific non-fatal warning issued */
#define FN_ERR_NO_DEVICE        (0x05)      /* There is no network device */

#define FN_ERR_UNKNOWN          (0xff)      /* Device specific error we didn't handle */

enum {
  NETWORK_SUCCESS                               = 1,
  NETWORK_ERROR_WRITE_ONLY                      = 131,
  NETWORK_ERROR_INVALID_COMMAND                 = 132,
  NETWORK_ERROR_READ_ONLY                       = 135,
  NETWORK_ERROR_END_OF_FILE                     = 136,
  NETWORK_ERROR_GENERAL_TIMEOUT                 = 138,
  NETWORK_ERROR_GENERAL                         = 144,
  NETWORK_ERROR_NOT_IMPLEMENTED                 = 146,
  NETWORK_ERROR_FILE_EXISTS                     = 151,
  NETWORK_ERROR_NO_SPACE_ON_DEVICE              = 162,
  NETWORK_ERROR_INVALID_DEVICESPEC              = 165,
  NETWORK_ERROR_ACCESS_DENIED                   = 167,
  NETWORK_ERROR_FILE_NOT_FOUND                  = 170,
  NETWORK_ERROR_CONNECTION_REFUSED              = 200,
  NETWORK_ERROR_NETWORK_UNREACHABLE             = 201,
  NETWORK_ERROR_SOCKET_TIMEOUT                  = 202,
  NETWORK_ERROR_NETWORK_DOWN                    = 203,
  NETWORK_ERROR_CONNECTION_RESET                = 204,
  NETWORK_ERROR_CONNECTION_ALREADY_IN_PROGRESS  = 205,
  NETWORK_ERROR_ADDRESS_IN_USE                  = 206,
  NETWORK_ERROR_NOT_CONNECTED                   = 207,
  NETWORK_ERROR_SERVER_NOT_RUNNING              = 208,
  NETWORK_ERROR_NO_CONNECTION_WAITING           = 209,
  NETWORK_ERROR_SERVICE_NOT_AVAILABLE           = 210,
  NETWORK_ERROR_CONNECTION_ABORTED              = 211,
  NETWORK_ERROR_INVALID_USERNAME_OR_PASSWORD    = 212,
  NETWORK_ERROR_COULD_NOT_PARSE_JSON            = 213,
  NETWORK_ERROR_CLIENT_GENERAL                  = 214,
  NETWORK_ERROR_SERVER_GENERAL                  = 215,
  NETWORK_ERROR_NO_DEVICE_AVAILABLE             = 216,
  NETWORK_ERROR_NOT_A_DIRECTORY                 = 217,
  NETWORK_ERROR_COULD_NOT_ALLOCATE_BUFFERS      = 255,
};

/**
 * Device specific error. This is the raw code from any device errors before they are converted to
 * simpler device-agnostic network library errors.
 */
extern uint8_t fn_device_error;

#endif /* FUJINET_ERR_H */

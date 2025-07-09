#ifndef FUJINET_ERR_H
#define FUJINET_ERR_H

#define FN_ERR_OK               (0x00)      /* No error */
#define FN_ERR_IO_ERROR         (0x01)      /* There was IO error/issue with the device */
#define FN_ERR_BAD_CMD          (0x02)      /* Function called with bad arguments */
#define FN_ERR_OFFLINE          (0x03)      /* The device is offline */
#define FN_ERR_WARNING          (0x04)      /* Device specific non-fatal warning issued */
#define FN_ERR_NO_DEVICE        (0x05)      /* There is no network device */

#define FN_ERR_UNKNOWN          (0xff)      /* Device specific error we didn't handle */

#endif /* FUJINET_ERR_H */

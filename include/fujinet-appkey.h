#ifndef FUJINET_APPKEY_H
#define FUJINET_APPKEY_H

#ifdef _CMOC_VERSION_
#include <fujinet-const.h>
typedef struct {
  uint16_t length;
  uint8_t data[MAX_APPKEY_LEN];
} FNAppKeyString;
extern FNAppKeyString appkey_buf;

#define fuji_bus_appkey_write(string, length) \
  FUJICALL_B12_D(FUJICMD_WRITE_APPKEY, length, string, MAX_APPKEY_LEN)

/*
  appkeys are variable length strings. CoCo drivewire is serial but
  drivewireFuji in the FujiNet firmware seems to be using fixed length
  packets with a header to indicate the true length of the key, same
  as Atari SIO. On write the aux1/aux2 fields are combined into a
  uint16_t. On read, there is an extra 2 bytes of header to indicate
  how much of the fixed block represents the string.
*/
#define fuji_bus_appkey_read(string, length) \
  (uint8_t) (FUJICALL_RV(FUJICMD_READ_APPKEY, &appkey_buf, sizeof(appkey_buf)) ? \
             (*(length) = appkey_buf.length,                            \
              memmove((string), appkey_buf.data, appkey_buf.length),    \
              true) : false)
#else /* ! _CMOC_VERSION */

extern bool fuji_bus_appkey_read(void *string, uint16_t *length);
extern bool fuji_bus_appkey_write(void *string, uint16_t length);

#endif /* _CMOC_VERSION_ */

#endif /* FUJINET_APPKEY_H */

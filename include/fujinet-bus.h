#ifndef FUJINET_BUS_H
#define FUJINET_BUS_H

// Convenience macros to fill in FujiDCB and make the call
#include <fujinet-bus-ezcall.h>

#ifdef _CMOC_VERSION_
    #include <cmoc.h>
    #include <coco.h>
    #include "stdbool-coco.h"
#else
    #include <stddef.h>
    #include <stdbool.h>
    #include <stdint.h>
#endif /* _CMOC_VERSION_ */

#define FUJI_DEVICEID_FUJINET           0x70

#define FUJI_DEVICEID_CLOCK             0x45
#define FUJI_DEVICEID_CPM               0x5A
#define FUJI_DEVICEID_DISK              0x31
#define FUJI_DEVICEID_DISK_LAST         0x3F
#define FUJI_DEVICEID_MIDI              0x99
#define FUJI_DEVICEID_NETWORK		0x71
#define FUJI_DEVICEID_NETWORK_LAST	0x78
#define FUJI_DEVICEID_PRINTER           0x40
#define FUJI_DEVICEID_PRINTER_LAST      0x43
#define FUJI_DEVICEID_SERIAL            0x50
#define FUJI_DEVICEID_SERIAL_LAST       0x53
#define FUJI_DEVICEID_VOICE		0x43

enum {
  FUJI_FIELD_NONE = 0x00,
  FUJI_FIELD_AUX1 = 0x01,
  FUJI_FIELD_AUX2 = 0x02,
  FUJI_FIELD_AUX3 = 0x04,
  FUJI_FIELD_AUX4 = 0x08,
};

extern bool fuji_bus_call(uint8_t device, uint8_t unit, uint8_t fuji_cmd, uint8_t fields,
			  uint8_t aux1, uint8_t aux2, uint8_t aux3, uint8_t aux4,
			  const void *data, size_t data_length,
			  void *reply, size_t reply_length);
extern size_t fuji_bus_read(uint8_t device, uint8_t unit, void *buffer, size_t length);
extern size_t fuji_bus_write(uint8_t device, uint8_t unit, const void *buffer, size_t length);

extern bool fuji_bus_appkey_read(void *string, uint16_t *length);
extern bool fuji_bus_appkey_write(void *string, uint16_t length);

#endif /* FUJINET_BUS_H */

#ifndef FUJINET_BUS_H
#define FUJINET_BUS_H

// Convenience wrappers to fill in FujiDCB and make the call
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

#define FUJI_DEVICEID_FUJINET 0x70

enum {
  FUJI_FIELD_NONE = 0x00,
  FUJI_FIELD_AUX1 = 0x01,
  FUJI_FIELD_AUX2 = 0x02,
  FUJI_FIELD_AUX3 = 0x04,
  FUJI_FIELD_AUX4 = 0x08,
};

extern bool fuji_bus_call(uint8_t fuji_cmd, uint8_t fields,
			  uint8_t aux1, uint8_t aux2, uint8_t aux3, uint8_t aux4,
			  const void *data, size_t data_length,
			  void *reply, size_t reply_length);

#endif /* FUJINET_BUS_H */

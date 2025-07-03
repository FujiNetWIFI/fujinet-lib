#ifndef FUJINET_BUS
#define FUJINET_BUS

#ifndef _CMOC_VERSION_
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#else /* _CMOC_VERSION_ */
#include <coco.h>
#include <stdbool-coco.h>
#endif /* _CMOC_VERSION_ */

extern bool fuji_bus_call(uint8_t fuji_cmd);
extern bool fuji_bus_get(void *data, size_t length);
extern bool fuji_bus_put(void *data, size_t length);

#endif /* FUJINET_BUS */

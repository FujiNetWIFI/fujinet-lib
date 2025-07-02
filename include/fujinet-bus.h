#ifndef FUJINET_BUS
#define FUJINET_BUS

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

extern bool transaction_call(uint8_t fuji_cmd);
extern bool transaction_get(void *data, size_t length);
extern bool transaction_put(void *data, size_t length);

#endif /* FUJINET_BUS */

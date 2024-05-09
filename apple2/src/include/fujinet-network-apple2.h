#ifndef FUJINET_NETWORK_APPLE2_H
#define FUJINET_NETWORK_APPLE2_H

#include <stdint.h>

// These are for Apple2 NETWORK C headers for internal and not exposed in the normal fujinet header files

uint8_t network_status_no_clr(char *devicespec, uint16_t *bw, uint8_t *c, uint8_t *err);
uint8_t network_unit(char *devicespec);

#endif
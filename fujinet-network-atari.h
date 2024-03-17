#ifndef FUJINET_NETWORK_ATARI_H
#define FUJINET_NETWORK_ATARI_H

#include <stdint.h>

// These are for ATARI NETWORK C headers for internal and not exposed in the normal fujinet header files

uint8_t network_status_unit(uint8_t unit, uint16_t *bw, uint8_t *c, uint8_t *err);
uint8_t network_unit(char *devicespec);

#endif
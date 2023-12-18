/**
 * @brief FujiNet Network Device Library Atari Base Functions
 * @license gpl v. 3, see LICENSE for details.
 */

#ifndef FUJINET_NETWORK_ATARI_H
#define FUJINET_NETWORK_ATARI_H

#include <stdint.h>

// These are for C files to be able to access ASM functions, and values
// that are internal and not exposed in the normal fujinet-network.h header

uint8_t sio_read(uint8_t unit, void * buffer, uint16_t len);
uint8_t network_status_unit(uint8_t unit, uint16_t *bw, uint8_t *c, uint8_t *err);
uint8_t network_unit(char *devicespec);

#endif

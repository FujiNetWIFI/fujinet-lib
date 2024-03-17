
/**
 * @brief FujiNet Network Device Library Apple2 Base Functions
 * @license gpl v. 3, see LICENSE for details.
 */

#ifndef FUJINET_BUS_ATARI_H
#define FUJINET_BUS_ATARI_H

#include <stdint.h>

// These are for Atari SIO C headers for internal and not exposed in the normal fujinet header files

uint8_t sio_read(uint8_t unit, void * buffer, uint16_t len);

#endif

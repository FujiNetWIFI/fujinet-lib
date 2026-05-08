#ifndef FUJINET_NETWORK_APPLE2_H
#define FUJINET_NETWORK_APPLE2_H

#include <stdint.h>

// These are for Apple2 NETWORK C headers for internal and not exposed in the normal fujinet header files

#define NETCMD_SET_CHANNEL  (0xfa)

/**
 * @brief Set Network unit, given unit
 * @param unit int containing Nx.
 */
void network_set_unit(uint8_t unit);

#endif
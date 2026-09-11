#ifndef FUJINET_BUS_EZCALL_H
#define FUJINET_BUS_EZCALL_H

#include <fujinet-endian.h>

enum {
  FUJI_FIELD_NONE        = 0,
  FUJI_FIELD_A1          = 1,
  FUJI_FIELD_A1_A2       = 2,
  FUJI_FIELD_A1_A2_A3    = 3,
  FUJI_FIELD_A1_A2_A3_A4 = 4,
  FUJI_FIELD_B12         = 5,
  FUJI_FIELD_B12_B34     = 6,
  FUJI_FIELD_C1234       = 7,
  FUJI_FIELD_DATA        = 8,
  FUJI_FIELD_REPLY       = 16,
};

/* Fuji Call Macro Naming Convention:
 *
 *   A#      = individual 8-bit aux fields (e.g. A1, A1_A2)
 *   B##     = combined 16-bit value packed into two aux fields
 *             - B12 = aux1 and aux2 combined as uint16_t
 *             - B34 = aux3 and aux4 combined as uint16_t
 *   C####   = combined 32-bit value packed into four aux fields
 *             - C1234 = aux1 through aux4 combined as uint32_t
 *   D       = includes data pointer and length
 *   RV      = includes pointer to reply pointer and reply_length
 *
 * Examples:
 *   DEVCALL_A1(dev, cmd, a1)
 *     aux1 = a1
 *
 *   DEVCALL_A1_A2(dev, cmd, a1, a2)
 *     aux1 = a1, aux2 = a2 (8-bit each)
 *
 *   DEVCALL_B12(dev, cmd, u16)
 *     aux1 = low byte, aux2 = high byte of u16
 *
 *   DEVCALL_C1234(dev, cmd, u32)
 *     aux1 = byte 0, aux2 = byte 1, aux3 = byte 2, aux4 = byte 3
 *
 *   DEVCALL_B12_D(dev, cmd, u16, data, len)
 *     aux1/aux2 = u16, and data block included
 */

#if defined(BUILD_ATARI)
#include <fujinet-bus-ezatari.h>
#elif FUJI_BUS_CALL_VARARGS
#include <fujinet-bus-ezvar.h>
#elif !FUJI_BUS_CALL_VARARGS
#include <fujinet-bus-ezfixed.h>
#else
#error "What type of fuji_bus_call is this"
#endif /* ! BUILD_ATARI */

// Sends a variable length packet with data on systems that have
// variable length packets, otherwise uses the aux1/aux2 bytes to
// encode the length of the data using native byte endianness.
extern bool fuji_devcall_vld(uint8_t device, uint8_t cmd, const void *data, uint16_t data_len);
#define DEVCALL_VLD(dev, cmd, data, len) fuji_devcall_vld(dev, cmd, data, len)

#include <fujinet-bus-ezfuji.h>
#include <fujinet-bus-eznet.h>
#include <fujinet-bus-ezclk.h>

#endif /* FUJINET_BUS_EZCALL_H */

#ifndef FUJINET_BUS
#define FUJINET_BUS

#ifdef _CMOC_VERSION_
    #include <cmoc.h>
    #include <coco.h>
    #include "stdbool-coco.h"
#else
    #include <stddef.h>
    #include <stdbool.h>
    #include <stdint.h>
#endif /* _CMOC_VERSION_ */

enum {
  FUJI_FIELD_NONE = 0x00,
  FUJI_FIELD_AUX1 = 0x01,
  FUJI_FIELD_AUX2 = 0x02,
  FUJI_FIELD_AUX3 = 0x04,
  FUJI_FIELD_AUX4 = 0x08,
  FUJI_FIELD_DATA = 0x10,
};

extern bool fuji_bus_call(uint8_t fuji_cmd, uint8_t fields,
                          uint8_t aux1, uint8_t aux2,
                          const void *data, size_t length);
extern bool fuji_bus_call_ext(uint8_t fuji_cmd, uint8_t fields,
                              uint8_t aux1, uint8_t aux2,
                              uint8_t aux3, uint8_t aux4,
                              const void *data, size_t length);
extern bool fuji_bus_get(void *data, size_t length);

/* Fuji Call Macro Naming Convention:
 *
 *   A#      = individual 8-bit aux fields (e.g. A1, A1_A2)
 *   B##     = combined 16-bit value packed into two aux fields
 *             - B12 = aux1 and aux2 combined as uint16_t
 *             - B34 = aux3 and aux4 combined as uint16_t
 *   C####   = combined 32-bit value packed into four aux fields
 *             - C1234 = aux1 through aux4 combined as uint32_t
 *   D       = includes data pointer and length
 *
 * Examples:
 *   FUJICALL_A1(cmd, a1)
 *     aux1 = a1
 *
 *   FUJICALL_A1_A2(cmd, a1, a2)
 *     aux1 = a1, aux2 = a2 (8-bit each)
 *
 *   FUJICALL_B12(cmd, u16)
 *     aux1 = low byte, aux2 = high byte of u16
 *
 *   FUJICALL_C1234(cmd, u32)
 *     aux1 = byte 0, aux2 = byte 1, aux3 = byte 2, aux4 = byte 3
 *
 *   FUJICALL_B12_D(cmd, u16, data, len)
 *     aux1/aux2 = u16, and data block included
 */

#define FUJICALL(cmd) \
  fuji_bus_call((cmd), 0, 0, 0, NULL, 0)

#define FUJICALL_D(cmd, data, len) \
  fuji_bus_call((cmd), FUJI_FIELD_DATA, 0, 0, data, len)

#define FUJICALL_A1(cmd, a1) \
  fuji_bus_call((cmd), FUJI_FIELD_AUX1, a1, 0, NULL, 0)

#define FUJICALL_A1_D(cmd, a1, data, len) \
  fuji_bus_call((cmd), FUJI_FIELD_AUX1 | FUJI_FIELD_DATA, a1, 0, data, len)

#define FUJICALL_A1_A2(cmd, a1, a2) \
  fuji_bus_call((cmd), FUJI_FIELD_AUX1 | FUJI_FIELD_AUX2, a1, a2, NULL, 0)

#define FUJICALL_A1_A2_D(cmd, a1, a2, data, len) \
  fuji_bus_call((cmd), FUJI_FIELD_AUX1 | FUJI_FIELD_AUX2 | FUJI_FIELD_DATA, a1, a2, data, len)

#define FUJICALL_A1_A2_A3(cmd, a1, a2, a3) \
  fuji_bus_call_ext((cmd), FUJI_FIELD_AUX1 | FUJI_FIELD_AUX2 | FUJI_FIELD_AUX3, \
                    a1, a2, a3, 0, NULL, 0)

#define FUJICALL_A1_A2_A3_D(cmd, a1, a2, a3, data, len)      \
  fuji_bus_call_ext((cmd), FUJI_FIELD_AUX1 | FUJI_FIELD_AUX2 \
                    | FUJI_FIELD_AUX3 | FUJI_FIELD_DATA, a1, a2, a3, 0, data, len)

#define FUJICALL_A1_A2_A3_A4(cmd, a1, a2, a3, a4)            \
  fuji_bus_call_ext((cmd), FUJI_FIELD_AUX1 | FUJI_FIELD_AUX2 \
                    | FUJI_FIELD_AUX3 | FUJI_FIELD_AUX4, a1, a2, a3, a4, NULL, 0)

#define FUJICALL_A1_A2_A3_A4_D(cmd, a1, a2, a3, a4, data, len)             \
  fuji_bus_call_ext((cmd), FUJI_FIELD_AUX1 | FUJI_FIELD_AUX2               \
                    | FUJI_FIELD_AUX3 | FUJI_FIELD_AUX4 | FUJI_FIELD_DATA, \
                    a1, a2, a3, a4, data, len)

#define FUJICALL_B12(cmd, val16)                                        \
  fuji_bus_call((cmd), FUJI_FIELD_AUX1 | FUJI_FIELD_AUX2,               \
                (uint8_t) ((val16) & 0xFF), (uint8_t) (((val16) >> 8) & 0xFF), NULL, 0)

#define FUJICALL_B12_D(cmd, val16, data, len)                               \
  fuji_bus_call((cmd), FUJI_FIELD_AUX1 | FUJI_FIELD_AUX2 | FUJI_FIELD_DATA, \
                (uint8_t) ((val16) & 0xFF), (uint8_t) (((val16) >> 8) & 0xFF), data, len)

#define FUJICALL_C1234(cmd, val32)                                                          \
  fuji_bus_call_ext((cmd), FUJI_FIELD_AUX1 | FUJI_FIELD_AUX2                                \
                    | FUJI_FIELD_AUX3 | FUJI_FIELD_AUX4,                                    \
                    (uint8_t) ((val32) & 0xFF), (uint8_t) (((val32) >> 8) & 0xFF),          \
                    (uint8_t) (((val32) >> 16) & 0xFF), (uint8_t) (((val32) >> 24) & 0xFF), \
                    NULL, 0)

#define FUJICALL_C1234_D(cmd, val32, data, len)                                             \
  fuji_bus_call_ext((cmd), FUJI_FIELD_AUX1 | FUJI_FIELD_AUX2                                \
                    | FUJI_FIELD_AUX3 | FUJI_FIELD_AUX4 | FUJI_FIELD_DATA,                  \
                    (uint8_t) ((val32) & 0xFF), (uint8_t) (((val32) >> 8) & 0xFF),          \
                    (uint8_t) (((val32) >> 16) & 0xFF), (uint8_t) (((val32) >> 24) & 0xFF), \
                    data, len)

#endif /* FUJINET_BUS */

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

#define DEVCALL(dev, cmd)                                                                   \
  fuji_bus_call(dev, cmd, FUJI_FIELD_NONE, 0, 0, NULL, 0)

#define DEVCALL_D(dev, cmd, data, len)                                                      \
  fuji_bus_call(dev, cmd, FUJI_FIELD_NONE | FUJI_FIELD_DATA, 0, 0, data, len)

#define DEVCALL_RV(dev, cmd, reply, replylen)                                               \
  fuji_bus_call(dev, cmd, FUJI_FIELD_NONE | FUJI_FIELD_REPLY, 0, 0, reply, replylen)

#define DEVCALL_A1(dev, cmd, a1)                                                            \
  fuji_bus_call(dev, cmd, FUJI_FIELD_A1, a1, 0, NULL, 0)

#define DEVCALL_A1_D(dev, cmd, a1, data, len)                                               \
  fuji_bus_call(dev, cmd, FUJI_FIELD_A1 | FUJI_FIELD_DATA, a1, 0, data, len)

#define DEVCALL_A1_RV(dev, cmd, a1, reply, replylen)                                        \
  fuji_bus_call(dev, cmd, FUJI_FIELD_A1 | FUJI_FIELD_REPLY, a1, 0, reply, replylen)

#define DEVCALL_A1_A2(dev, cmd, a1, a2)                                                     \
  fuji_bus_call(dev, cmd, FUJI_FIELD_A1_A2, a1, a2, NULL, 0)

#define DEVCALL_A1_A2_D(dev, cmd, a1, a2, data, len)                                        \
  fuji_bus_call(dev, cmd, FUJI_FIELD_A1_A2 | FUJI_FIELD_DATA, a1, a2, data, len)

#define DEVCALL_A1_A2_RV(dev, cmd, a1, a2, reply, replylen)                                 \
  fuji_bus_call(dev, cmd, FUJI_FIELD_A1_A2 | FUJI_FIELD_REPLY, a1, a2, reply, replylen)

#define DEVCALL_B12(dev, cmd, b12)                                                          \
  fuji_bus_call(dev, cmd, FUJI_FIELD_B12, NATIVE_SPLIT_U16(b12), NULL, 0)

#define DEVCALL_B12_D(dev, cmd, b12, data, len)                                             \
  fuji_bus_call(dev, cmd, FUJI_FIELD_B12 | FUJI_FIELD_DATA, NATIVE_SPLIT_U16(b12), data, len)

#define DEVCALL_B12_RV(dev, cmd, b12, reply, replylen)                                      \
  fuji_bus_call(dev, cmd, FUJI_FIELD_B12 | FUJI_FIELD_REPLY, NATIVE_SPLIT_U16(b12),         \
                reply, replylen)

#elif defined(BUILD_MSDOS) || defined(BUILD_ADAM) || defined(BUILD_LYNX)

#define DEVCALL(dev, cmd)                                                                   \
  fuji_bus_call(dev, cmd, FUJI_FIELD_NONE, 0, 0, 0, 0, NULL, 0)

#define DEVCALL_D(dev, cmd, data, len)                                                      \
  fuji_bus_call(dev, cmd, FUJI_FIELD_NONE | FUJI_FIELD_DATA, 0, 0, 0, 0, data, len)

#define DEVCALL_RV(dev, cmd, reply, replylen)                                               \
  fuji_bus_call(dev, cmd, FUJI_FIELD_NONE | FUJI_FIELD_REPLY, 0, 0, 0, 0, reply, replylen)

#define DEVCALL_A1(dev, cmd, a1)                                                            \
  fuji_bus_call(dev, cmd, FUJI_FIELD_A1, a1, 0, 0, 0, NULL, 0)

#define DEVCALL_A1_D(dev, cmd, a1, data, len)                                               \
  fuji_bus_call(dev, cmd, FUJI_FIELD_A1 | FUJI_FIELD_DATA, a1, 0, 0, 0, data, len)

#define DEVCALL_A1_RV(dev, cmd, a1, reply, replylen)                                        \
  fuji_bus_call(dev, cmd, FUJI_FIELD_A1 | FUJI_FIELD_REPLY, a1, 0, 0, 0, reply, replylen)

#define DEVCALL_A1_A2(dev, cmd, a1, a2)                                                     \
  fuji_bus_call(dev, cmd, FUJI_FIELD_A1_A2, a1, a2, 0, 0, NULL, 0)

#define DEVCALL_A1_A2_D(dev, cmd, a1, a2, data, len)                                        \
  fuji_bus_call(dev, cmd, FUJI_FIELD_A1_A2 | FUJI_FIELD_DATA, a1, a2, 0, 0, data, len)

#define DEVCALL_A1_A2_RV(dev, cmd, a1, a2, reply, replylen)                                 \
  fuji_bus_call(dev, cmd, FUJI_FIELD_A1_A2 | FUJI_FIELD_REPLY, a1, a2, 0, 0, reply, replylen)

#define DEVCALL_A1_A2_A3(dev, cmd, a1, a2, a3)                                              \
  fuji_bus_call(dev, cmd, FUJI_FIELD_A1_A2_A3, a1, a2, a3, 0, NULL, 0)

#define DEVCALL_A1_A2_A3_D(dev, cmd, a1, a2, a3, data, len)                                 \
  fuji_bus_call(dev, cmd, FUJI_FIELD_A1_A2_A3 | FUJI_FIELD_DATA, a1, a2, a3, 0, data, len)

#define DEVCALL_A1_A2_A3_RV(dev, cmd, a1, a2, a3, reply, replylen)                          \
  fuji_bus_call(dev, cmd, FUJI_FIELD_A1_A2_A3 | FUJI_FIELD_REPLY, a1, a2, a3, 0,            \
                reply, replylen)

#define DEVCALL_A1_A2_A3_A4(dev, cmd, a1, a2, a3, a4)                                       \
  fuji_bus_call(dev, cmd, FUJI_FIELD_A1_A2_A3_A4, a1, a2, a3, a4, NULL, 0)

#define DEVCALL_A1_A2_A3_A4_D(dev, cmd, a1, a2, a3, a4, data, len)                          \
  fuji_bus_call(dev, cmd, FUJI_FIELD_A1_A2_A3_A4 | FUJI_FIELD_DATA, a1, a2, a3, a4, data, len)

#define DEVCALL_A1_A2_A3_A4_RV(dev, cmd, a1, a2, a3, a4, reply, replylen)                   \
  fuji_bus_call(dev, cmd, FUJI_FIELD_A1_A2_A3_A4 | FUJI_FIELD_REPLY, a1, a2, a3, a4,        \
                reply, replylen)

#define DEVCALL_B12(dev, cmd, b12)                                                          \
  fuji_bus_call(dev, cmd, FUJI_FIELD_B12, NATIVE_SPLIT_U16(b12), 0, 0, NULL, 0)

#define DEVCALL_B12_D(dev, cmd, b12, data, len)                                             \
  fuji_bus_call(dev, cmd, FUJI_FIELD_B12 | FUJI_FIELD_DATA, NATIVE_SPLIT_U16(b12), 0, 0,    \
                data, len)

#define DEVCALL_B12_RV(dev, cmd, b12, reply, replylen)                                      \
  fuji_bus_call(dev, cmd, FUJI_FIELD_B12 | FUJI_FIELD_REPLY, NATIVE_SPLIT_U16(b12), 0, 0,   \
                reply, replylen)

#define DEVCALL_B12_B34(dev, cmd, b12, b34)                                                 \
  fuji_bus_call(dev, cmd, FUJI_FIELD_B12_B34, NATIVE_SPLIT_U16(b12), NATIVE_SPLIT_U16(b34), \
                NULL, 0)

#define DEVCALL_B12_B34_D(dev, cmd, b12, b34, data, len)                                    \
  fuji_bus_call(dev, cmd, FUJI_FIELD_B12_B34 | FUJI_FIELD_DATA,                             \
                NATIVE_SPLIT_U16(b12), NATIVE_SPLIT_U16(b34), data, len)

#define DEVCALL_B12_B34_RV(dev, cmd, b12, b34, reply, replylen)                             \
  fuji_bus_call(dev, cmd, FUJI_FIELD_B12_B34 | FUJI_FIELD_REPLY,                            \
		NATIVE_SPLIT_U16(b12), NATIVE_SPLIT_U16(b34), reply, replylen)

#define DEVCALL_C1234(dev, cmd, c1234)                                                      \
  fuji_bus_call(dev, cmd, FUJI_FIELD_C1234, NATIVE_SPLIT_U32(U32_LSW(c1234)))

#define DEVCALL_C1234_D(dev, cmd, c1234, data, len)                                         \
  fuji_bus_call(dev, cmd, FUJI_FIELD_C1234 | FUJI_FIELD_DATA,                               \
                NATIVE_SPLIT_U32(U32_LSW(c1234)), data, len)

#define DEVCALL_C1234_RV(dev, cmd, c1234, reply, replylen)                                  \
  fuji_bus_call(dev, cmd, FUJI_FIELD_C1234 | FUJI_FIELD_REPLY,                              \
                NATIVE_SPLIT_U32(U32_LSW(c1234)), reply, replylen)

#else /* ! (BUILD_ATARI || BUILD_MSDOS || BUILD_ADAM) */

#define DEVCALL(dev, cmd)                                                                   \
  fuji_bus_call(dev, cmd, FUJI_FIELD_NONE)

#define DEVCALL_D(dev, cmd, data, len)                                                      \
  fuji_bus_call(dev, cmd, FUJI_FIELD_NONE | FUJI_FIELD_DATA, data, len)

#define DEVCALL_RV(dev, cmd, reply, replylen)                                               \
  fuji_bus_call(dev, cmd, FUJI_FIELD_NONE | FUJI_FIELD_REPLY, reply, replylen)

#define DEVCALL_A1(dev, cmd, a1)                                                            \
  fuji_bus_call(dev, cmd, FUJI_FIELD_A1, a1)

#define DEVCALL_A1_D(dev, cmd, a1, data, len)                                               \
  fuji_bus_call(dev, cmd, FUJI_FIELD_A1 | FUJI_FIELD_DATA, a1, data, len)

#define DEVCALL_A1_RV(dev, cmd, a1, reply, replylen)                                        \
  fuji_bus_call(dev, cmd, FUJI_FIELD_A1 | FUJI_FIELD_REPLY, a1, reply, replylen)

#define DEVCALL_A1_A2(dev, cmd, a1, a2)                                                     \
  fuji_bus_call(dev, cmd, FUJI_FIELD_A1_A2, a1, a2)

#define DEVCALL_A1_A2_D(dev, cmd, a1, a2, data, len)                                        \
  fuji_bus_call(dev, cmd, FUJI_FIELD_A1_A2 | FUJI_FIELD_DATA, a1, a2, data, len)

#define DEVCALL_A1_A2_RV(dev, cmd, a1, a2, reply, replylen)                                 \
  fuji_bus_call(dev, cmd, FUJI_FIELD_A1_A2 | FUJI_FIELD_REPLY, a1, a2, reply, replylen)

#define DEVCALL_A1_A2_A3(dev, cmd, a1, a2, a3)                                              \
  fuji_bus_call(dev, cmd, FUJI_FIELD_A1_A2_A3, a1, a2, a3)

#define DEVCALL_A1_A2_A3_D(dev, cmd, a1, a2, a3, data, len)                                 \
  fuji_bus_call(dev, cmd, FUJI_FIELD_A1_A2_A3 | FUJI_FIELD_DATA, a1, a2, a3, data, len)

#define DEVCALL_A1_A2_A3_RV(dev, cmd, a1, a2, a3, reply, replylen)                          \
  fuji_bus_call(dev, cmd, FUJI_FIELD_A1_A2_A3 | FUJI_FIELD_REPLY, a1, a2, a3, reply, replylen)

#define DEVCALL_A1_A2_A3_A4(dev, cmd, a1, a2, a3, a4)                                       \
  fuji_bus_call(dev, cmd, FUJI_FIELD_A1_A2_A3_A4, a1, a2, a3, a4)

#define DEVCALL_A1_A2_A3_A4_D(dev, cmd, a1, a2, a3, a4, data, len)                          \
  fuji_bus_call(dev, cmd, FUJI_FIELD_A1_A2_A3_A4 | FUJI_FIELD_DATA, a1, a2, a3, a4, data, len)

#define DEVCALL_A1_A2_A3_A4_RV(dev, cmd, a1, a2, a3, a4, reply, replylen)                   \
  fuji_bus_call(dev, cmd, FUJI_FIELD_A1_A2_A3_A4 | FUJI_FIELD_REPLY, a1, a2, a3, a4,        \
                reply, replylen)

#define DEVCALL_B12(dev, cmd, b12)                                                          \
  fuji_bus_call(dev, cmd, FUJI_FIELD_B12, NATIVE_SPLIT_U16(b12))

#define DEVCALL_B12_D(dev, cmd, b12, data, len)                                             \
  fuji_bus_call(dev, cmd, FUJI_FIELD_B12 | FUJI_FIELD_DATA, NATIVE_SPLIT_U16(b12), data, len)

#define DEVCALL_B12_RV(dev, cmd, b12, reply, replylen)                                      \
  fuji_bus_call(dev, cmd, FUJI_FIELD_B12 | FUJI_FIELD_REPLY, NATIVE_SPLIT_U16(b12),         \
                reply, replylen)

#define DEVCALL_B12_B34(dev, cmd, b12, b34)                                                 \
  fuji_bus_call(dev, cmd, FUJI_FIELD_B12_B34, NATIVE_SPLIT_U16(b12), NATIVE_SPLIT_U16(b34))

#define DEVCALL_B12_B34_D(dev, cmd, b12, b34, data, len)                                    \
  fuji_bus_call(dev, cmd, FUJI_FIELD_B12_B34 | FUJI_FIELD_DATA,                             \
                NATIVE_SPLIT_U16(b12), NATIVE_SPLIT_U16(b34), data, len)

#define DEVCALL_B12_B34_RV(dev, cmd, b12, b34, reply, replylen)                             \
  fuji_bus_call(dev, cmd, FUJI_FIELD_B12_B34 | FUJI_FIELD_REPLY,                            \
		NATIVE_SPLIT_U16(b12), NATIVE_SPLIT_U16(b34), reply, replylen)

#define DEVCALL_C1234(dev, cmd, c1234)                                                      \
  fuji_bus_call(dev, cmd, FUJI_FIELD_C1234, NATIVE_SPLIT_U32(U32_LSW(c1234)))

#define DEVCALL_C1234_D(dev, cmd, c1234, data, len)                                         \
  fuji_bus_call(dev, cmd, FUJI_FIELD_C1234 | FUJI_FIELD_DATA,                               \
                NATIVE_SPLIT_U32(U32_LSW(c1234)), data, len)

#define DEVCALL_C1234_RV(dev, cmd, c1234, reply, replylen)                                  \
  fuji_bus_call(dev, cmd, FUJI_FIELD_C1234 | FUJI_FIELD_REPLY,                              \
                NATIVE_SPLIT_U32(U32_LSW(c1234)), reply, replylen)

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

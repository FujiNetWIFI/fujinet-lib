#ifndef FUJINET_BUS_EZCALL_H
#define FUJINET_BUS_EZCALL_H

#include <fujinet-endian.h>

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
  fuji_bus_call(FUJI_DEVICEID_FUJINET, 1, cmd, FUJI_FIELD_NONE, 0, 0, 0, 0, NULL, 0, NULL, 0)

#define FUJICALL_D(cmd, data, len) \
  fuji_bus_call(FUJI_DEVICEID_FUJINET, 1, cmd, 0, 0, 0, 0, 0, data, len, NULL, 0)

#define FUJICALL_RV(cmd, reply, replylen) \
  fuji_bus_call(FUJI_DEVICEID_FUJINET, 1, cmd, FUJI_FIELD_NONE, \
		0, 0, 0, 0, NULL, 0, reply, replylen)

#define FUJICALL_A1(cmd, a1) \
  fuji_bus_call(FUJI_DEVICEID_FUJINET, 1, cmd, FUJI_FIELD_AUX1, a1, 0, 0, 0, NULL, 0, NULL, 0)

#define FUJICALL_A1_A2(cmd, a1, a2) \
  fuji_bus_call(FUJI_DEVICEID_FUJINET, 1, cmd, \
		FUJI_FIELD_AUX1 | FUJI_FIELD_AUX2, a1, a2, 0, 0, NULL, 0, NULL, 0)

#define FUJICALL_A1_A2_A3(cmd, a1, a2, a3) \
  fuji_bus_call(FUJI_DEVICEID_FUJINET, 1, cmd, \
		FUJI_FIELD_AUX1 | FUJI_FIELD_AUX2 | FUJI_FIELD_AUX3,	\
		a1, a2, a3, 0, NULL, 0, NULL, 0)

#define FUJICALL_A1_A2_A3_A4(cmd, a1, a2, a3, a4) \
  fuji_bus_call(FUJI_DEVICEID_FUJINET, 1, cmd, \
		FUJI_FIELD_AUX1 | FUJI_FIELD_AUX2 | FUJI_FIELD_AUX3 | FUJI_FIELD_AUX4, \
		a1, a2, a3, a4, NULL, 0, NULL, 0)

#define FUJICALL_A1_A2_A3_A4_D(cmd, a1, a2, a3, a4, data, len) \
  fuji_bus_call(FUJI_DEVICEID_FUJINET, 1, cmd, \
		FUJI_FIELD_AUX1 | FUJI_FIELD_AUX2 | FUJI_FIELD_AUX3 | FUJI_FIELD_AUX4, \
		a1, a2, a3, a4, data, len, NULL, 0)

#define FUJICALL_A1_A2_A3_A4_RV(cmd, a1, a2, a3, a4, reply, replylen) \
  fuji_bus_call(FUJI_DEVICEID_FUJINET, 1, cmd, \
		FUJI_FIELD_AUX1 | FUJI_FIELD_AUX2 | FUJI_FIELD_AUX3 | FUJI_FIELD_AUX4 \
		, a1, a2, a3, a4, NULL, 0, reply, replylen)

#define FUJICALL_A1_A2_A3_D(cmd, a1, a2, a3, data, len) \
  fuji_bus_call(FUJI_DEVICEID_FUJINET, 1, cmd, \
		FUJI_FIELD_AUX1 | FUJI_FIELD_AUX2 | FUJI_FIELD_AUX3,	\
		a1, a2, a3, 0, data, len, NULL, 0)

#define FUJICALL_A1_A2_A3_RV(cmd, a1, a2, a3, reply, replylen) \
  fuji_bus_call(FUJI_DEVICEID_FUJINET, 1, cmd, \
		FUJI_FIELD_AUX1 | FUJI_FIELD_AUX2 | FUJI_FIELD_AUX3,	\
		a1, a2, a3, 0, NULL, 0, reply, replylen)

#define FUJICALL_A1_A2_D(cmd, a1, a2, data, len) \
  fuji_bus_call(FUJI_DEVICEID_FUJINET, 1, cmd, FUJI_FIELD_AUX1 | FUJI_FIELD_AUX2, \
		a1, a2, 0, 0, data, len, NULL, 0)

#define FUJICALL_A1_A2_RV(cmd, a1, a2, reply, replylen) \
  fuji_bus_call(FUJI_DEVICEID_FUJINET, 1, cmd, FUJI_FIELD_AUX1 | FUJI_FIELD_AUX2, \
		a1, a2, 0, 0, NULL, 0, reply, replylen)

#define FUJICALL_A1_D(cmd, a1, data, len) \
  fuji_bus_call(FUJI_DEVICEID_FUJINET, 1, cmd, FUJI_FIELD_AUX1, \
		a1, 0, 0, 0, data, len, NULL, 0)

#define FUJICALL_A1_RV(cmd, a1, reply, replylen) \
  fuji_bus_call(FUJI_DEVICEID_FUJINET, 1, cmd, FUJI_FIELD_AUX1, \
		a1, 0, 0, 0, NULL, 0, reply, replylen)

#define FUJICALL_B12(cmd, b12) \
  FUJICALL_A1_A2(cmd, U16_LSB(b12), U16_MSB(b12))

#define FUJICALL_B12_D(cmd, b12, data, len) \
  FUJICALL_A1_A2_D(cmd, U16_LSB(b12), U16_MSB(b12), data, len)

#define FUJICALL_B12_RV(cmd, b12, reply, replylen) \
  FUJICALL_A1_A2_RV(cmd, U16_LSB(b12), U16_MSB(b12), reply, replylen)

#define FUJICALL_B12_B34(cmd, b12, b34)				\
  FUJICALL_A1_A2_A3_A4(cmd, U16_LSB(b12), U16_MSB(b12), U16_LSB(b34), U16_MSB(b34))

#define FUJICALL_B12_B34_D(cmd, b12, b34, data, len)			\
  FUJICALL_A1_A2_A3_A4(cmd, U16_LSB(b12), U16_MSB(b12), U16_LSB(b34), U16_MSB(b34), data, len)

#define FUJICALL_B12_B34_RV(cmd, b12, b34, reply, replylen)	\
  FUJICALL_A1_A2_A3_A4_RV(cmd, U16_LSB(b12), U16_MSB(b12), U16_LSB(b34), U16_MSB(b34), \
                          reply, replylen)

#define FUJICALL_C1234(cmd, c1234) \
  FUJICALL_B12_B34(cmd, U32_LSW(c1234), U32_MSW(c1234))

#define FUJICALL_C1234_D(cmd, c1234, data, len) \
  FUJICALL_B12_B34_D(U32_LSW(c1234), U32_MSW(c1234), data, len)

#define FUJICALL_C1234_RV(cmd, c1234, reply, replylen) \
  FUJICALL_B12_B34_RV(cmd, U32_LSW(c1234), U32_MSW(c1234), reply, replylen)

#include <fujinet-bus-eznet.h>

#endif /* FUJINET_BUS_EZCALL_H */

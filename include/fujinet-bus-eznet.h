
#define NETCALL_A1_A2(cmd, unit, a1, a2) \
  fuji_bus_call(FUJI_DEVICEID_NETWORK, unit, cmd, \
		FUJI_FIELD_AUX1 | FUJI_FIELD_AUX2, a1, a2, 0, 0, NULL, 0, NULL, 0)

#define NETCALL_A1_A2_A3(cmd, unit, a1, a2, a3) \
  fuji_bus_call(FUJI_DEVICEID_NETWORK, unit, cmd, \
		FUJI_FIELD_AUX1 | FUJI_FIELD_AUX2 | FUJI_FIELD_AUX3,	\
		a1, a2, a3, 0, NULL, 0, NULL, 0)

#define NETCALL_A1_A2_A3_A4(cmd, unit, a1, a2, a3, a4) \
  fuji_bus_call(FUJI_DEVICEID_NETWORK, unit, cmd, \
		FUJI_FIELD_AUX1 | FUJI_FIELD_AUX2 | FUJI_FIELD_AUX3 | FUJI_FIELD_AUX4, \
		a1, a2, a3, a4, NULL, 0, NULL, 0)

#define NETCALL_A1_A2_A3_A4_D(cmd, unit, a1, a2, a3, a4, data, len) \
  fuji_bus_call(FUJI_DEVICEID_NETWORK, unit, cmd, \
		FUJI_FIELD_AUX1 | FUJI_FIELD_AUX2 | FUJI_FIELD_AUX3 | FUJI_FIELD_AUX4, \
		a1, a2, a3, a4, data, len, NULL, 0)

#define NETCALL_A1_A2_A3_A4_RV(cmd, unit, a1, a2, a3, a4, reply, replylen) \
  fuji_bus_call(FUJI_DEVICEID_NETWORK, unit, cmd, \
		FUJI_FIELD_AUX1 | FUJI_FIELD_AUX2 | FUJI_FIELD_AUX3 | FUJI_FIELD_AUX4 \
		, a1, a2, a3, a4, NULL, 0, reply, replylen)

#define NETCALL_A1_A2_A3_D(cmd, unit, a1, a2, a3, data, len) \
  fuji_bus_call(FUJI_DEVICEID_NETWORK, unit, cmd, \
		FUJI_FIELD_AUX1 | FUJI_FIELD_AUX2 | FUJI_FIELD_AUX3,	\
		a1, a2, a3, 0, data, len, NULL, 0)

#define NETCALL_A1_A2_A3_RV(cmd, unit, a1, a2, a3, reply, replylen) \
  fuji_bus_call(FUJI_DEVICEID_NETWORK, unit, cmd, \
		FUJI_FIELD_AUX1 | FUJI_FIELD_AUX2 | FUJI_FIELD_AUX3,	\
		a1, a2, a3, 0, NULL, 0, reply, replylen)

#define NETCALL_A1_A2_D(cmd, unit, a1, a2, data, len) \
  fuji_bus_call(FUJI_DEVICEID_NETWORK, unit, cmd, FUJI_FIELD_AUX1 | FUJI_FIELD_AUX2, \
		a1, a2, 0, 0, data, len, NULL, 0)

#define NETCALL_A1_A2_RV(cmd, unit, a1, a2, reply, replylen) \
  fuji_bus_call(FUJI_DEVICEID_NETWORK, unit, cmd, FUJI_FIELD_AUX1 | FUJI_FIELD_AUX2, \
		a1, a2, 0, 0, NULL, 0, reply, replylen)

#define NETCALL_A1_D(cmd, unit, a1, data, len) \
  fuji_bus_call(FUJI_DEVICEID_NETWORK, unit, cmd, FUJI_FIELD_AUX1, \
		a1, 0, 0, 0, data, len, NULL, 0)

#define NETCALL_A1_RV(cmd, unit, a1, reply, replylen) \
  fuji_bus_call(FUJI_DEVICEID_NETWORK, unit, cmd, FUJI_FIELD_AUX1, \
		a1, 0, 0, 0, NULL, 0, reply, replylen)

#define NETCALL_B12(cmd, unit, b12) \
  NETCALL_A1_A2(cmd, unit, U16_LSB(b12), U16_MSB(b12))

#define NETCALL_B12_D(cmd, unit, b12, data, len) \
  NETCALL_A1_A2_D(cmd, unit, U16_LSB(b12), U16_MSB(b12), data, len)

#define NETCALL_B12_RV(cmd, unit, b12, reply, replylen) \
  NETCALL_A1_A2_RV(cmd, unit, U16_LSB(b12), U16_MSB(b12), reply, replylen)

#define NETCALL_B12_B34(cmd, unit, b12, b34)				\
  NETCALL_A1_A2_A3_A4(cmd, unit, U16_LSB(b12), U16_MSB(b12), U16_LSB(b34), U16_MSB(b34))

#define NETCALL_B12_B34_D(cmd, unit, b12, b34, data, len)			\
  NETCALL_A1_A2_A3_A4_D(cmd, unit, U16_LSB(b12), U16_MSB(b12), U16_LSB(b34), U16_MSB(b34), \
                        data, len)

#define NETCALL_B12_B34_RV(cmd, unit, b12, b34, reply, replylen)	\
  NETCALL_A1_A2_A3_A4_RV(cmd, unit, U16_LSB(b12), U16_MSB(b12), U16_LSB(b34), U16_MSB(b34), \
                         reply, replylen)

#define NETCALL_C1234(cmd, unit, c1234) \
  NETCALL_B12_B34(cmd, unit, U32_LSW(c1234), U32_MSW(c1234))

#define NETCALL_C1234_D(cmd, unit, c1234, data, len) \
  NETCALL_B12_B34_D(cmd, unit, U32_LSW(c1234), U32_MSW(c1234), data, len)

#define NETCALL_C1234_RV(cmd, unit, c1234, reply, replylen) \
  NETCALL_B12_B34_RV(cmd, unit, U32_LSW(c1234), U32_MSW(c1234), reply, replylen)

#ifdef _CMOC_VERSION_
// On CoCo all network calls *must* have aux1 and aux2
#define NETCALL(cmd, unit)                                              \
  NETCALL_A1_A2(cmd, unit, 0, 0)

#define NETCALL_D(cmd, unit, data, len) \
  NETCALL_A1_A2_D(cmd, unit, 0, 0, data, len)

#define NETCALL_RV(cmd, unit, reply, replylen) \
  NETCALL_A1_A2_RV(cmd, unit, 0, 0, reply, replylen)

#define NETCALL_A1(cmd, unit, a1) \
  NETCALL_A1_A2(cmd, unit, a1, 0)
#else /* ! _CMOC_VERSION_ */
#define NETCALL(cmd, unit) \
  fuji_bus_call(FUJI_DEVICEID_NETWORK, unit, cmd, FUJI_FIELD_NONE, 0, 0, 0, 0, NULL, 0, NULL, 0)

#define NETCALL_D(cmd, unit, data, len) \
  fuji_bus_call(FUJI_DEVICEID_NETWORK, unit, cmd, 0, 0, 0, 0, 0, data, len, NULL, 0)

#define NETCALL_RV(cmd, unit, reply, replylen) \
  fuji_bus_call(FUJI_DEVICEID_NETWORK, unit, cmd, FUJI_FIELD_NONE, \
		0, 0, 0, 0, NULL, 0, reply, replylen)

#define NETCALL_A1(cmd, unit, a1) \
  fuji_bus_call(FUJI_DEVICEID_NETWORK, unit, cmd, FUJI_FIELD_AUX1, a1, 0, 0, 0, NULL, 0, NULL, 0)
#endif /* _CMOC_VERSION_ */


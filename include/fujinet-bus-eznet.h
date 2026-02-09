#define NETCALL_A1_A2(cmd, unit, a1, a2)                                   \
  fuji_bus_call(FUJI_DEVICEID_NETWORK + (unit) - 1, cmd,                   \
		FUJI_FIELD_A1_A2,                                          \
                a1, a2, 0, 0,                                              \
                NULL, 0, NULL, 0)

#define NETCALL_A1_A2_D(cmd, unit, a1, a2, data, len)                      \
  fuji_bus_call(FUJI_DEVICEID_NETWORK + (unit) - 1, cmd,                   \
                FUJI_FIELD_A1_A2,                                          \
                a1, a2, 0, 0,                                              \
                data, len, NULL, 0)

#define NETCALL_A1_A2_RV(cmd, unit, a1, a2, reply, replylen)               \
  fuji_bus_call(FUJI_DEVICEID_NETWORK + (unit) - 1, cmd,                   \
                FUJI_FIELD_A1_A2,                                          \
                a1, a2, 0, 0,                                              \
                NULL, 0, reply, replylen)

#define NETCALL_A1_A2_A3(cmd, unit, a1, a2, a3)                            \
  fuji_bus_call(FUJI_DEVICEID_NETWORK + (unit) - 1, cmd,                   \
                FUJI_FIELD_A1_A2_A3,                                       \
		a1, a2, a3, 0,                                             \
                NULL, 0, NULL, 0)

#define NETCALL_A1_A2_A3_D(cmd, unit, a1, a2, a3, data, len)               \
  fuji_bus_call(FUJI_DEVICEID_NETWORK + (unit) - 1, cmd,                   \
                FUJI_FIELD_A1_A2_A3,                                       \
		a1, a2, a3, 0,                                             \
                data, len, NULL, 0)

#define NETCALL_A1_A2_A3_RV(cmd, unit, a1, a2, a3, reply, replylen)        \
  fuji_bus_call(FUJI_DEVICEID_NETWORK + (unit) - 1, cmd,                   \
                FUJI_FIELD_A1_A2_A3,                                       \
		a1, a2, a3, 0,                                             \
                NULL, 0, reply, replylen)

#define NETCALL_A1_A2_A3_A4(cmd, unit, a1, a2, a3, a4)                     \
  fuji_bus_call(FUJI_DEVICEID_NETWORK + (unit) - 1, cmd,                   \
                FUJI_FIELD_A1_A2_A3_A4,                                    \
		a1, a2, a3, a4,                                            \
                NULL, 0, NULL, 0)

#define NETCALL_A1_A2_A3_A4_D(cmd, unit, a1, a2, a3, a4, data, len)        \
  fuji_bus_call(FUJI_DEVICEID_NETWORK + (unit) - 1, cmd,                   \
                FUJI_FIELD_A1_A2_A3_A4,                                    \
		a1, a2, a3, a4,                                            \
                data, len, NULL, 0)

#define NETCALL_A1_A2_A3_A4_RV(cmd, unit, a1, a2, a3, a4, reply, replylen) \
  fuji_bus_call(FUJI_DEVICEID_NETWORK + (unit) - 1, cmd,                   \
                FUJI_FIELD_A1_A2_A3_A4,                                    \
                a1, a2, a3, a4,                                            \
                NULL, 0, reply, replylen)

#define NETCALL_B12(cmd, unit, b12)                                        \
  fuji_bus_call(FUJI_DEVICEID_NETWORK + (unit) - 1, cmd,                   \
		FUJI_FIELD_B12,                                            \
                U16_LSB(b12), U16_MSB(b12), 0, 0,                          \
                NULL, 0, NULL, 0)

#define NETCALL_B12_D(cmd, unit, b12, data, len)                           \
  fuji_bus_call(FUJI_DEVICEID_NETWORK + (unit) - 1, cmd,                   \
		FUJI_FIELD_B12,                                            \
                U16_LSB(b12), U16_MSB(b12), 0, 0,                          \
                data, len, NULL, 0)

#define NETCALL_B12_RV(cmd, unit, b12, reply, replylen)                    \
  fuji_bus_call(FUJI_DEVICEID_NETWORK + (unit) - 1, cmd,                   \
		FUJI_FIELD_B12,                                            \
                U16_LSB(b12), U16_MSB(b12), 0, 0,                          \
                NULL, 0, reply, replylen)

#define NETCALL_B12_B34(cmd, unit, b12, b34)                               \
  fuji_bus_call(FUJI_DEVICEID_NETWORK + (unit) - 1, cmd,                   \
                FUJI_FIELD_B12_B34,                                        \
                U16_LSB(b12), U16_MSB(b12), U16_LSB(b34), U16_MSB(b34),    \
                NULL, 0, NULL, 0)

#define NETCALL_B12_B34_D(cmd, unit, b12, b34, data, len)		   \
  fuji_bus_call(FUJI_DEVICEID_NETWORK + (unit) - 1, cmd,                   \
                FUJI_FIELD_B12_B34,                                        \
                U16_LSB(b12), U16_MSB(b12), U16_LSB(b34), U16_MSB(b34),    \
                data, len, NULL, 0)

#define NETCALL_B12_B34_RV(cmd, unit, b12, b34, reply, replylen)           \
  fuji_bus_call(FUJI_DEVICEID_NETWORK + (unit) - 1, cmd,                   \
                FUJI_FIELD_B12_b34,                                        \
                U16_LSB(b12), U16_MSB(b12), U16_LSB(b34), U16_MSB(b34),    \
                NULL, 0, reply, replylen)

#define NETCALL_C1234(cmd, unit, c1234)                                    \
  fuji_bus_call(FUJI_DEVICEID_NETWORK + (unit) - 1, cmd,                   \
                FUJI_FIELD_C1234,                                          \
                U16_LSB(U32_LSW(c1234)), U16_MSB(U32_LSW(c1234)),          \
                U16_LSB(U32_MSW(c1234)), U16_MSB(U32_MSW(c1234)),          \
                NULL, 0, NULL, 0)

#define NETCALL_C1234_D(cmd, unit, c1234, data, len)                       \
  fuji_bus_call(FUJI_DEVICEID_NETWORK + (unit) - 1, cmd,                   \
                FUJI_FIELD_C1234,                                          \
                U16_LSB(U32_LSW(c1234)), U16_MSB(U32_LSW(c1234)),          \
                U16_LSB(U32_MSW(c1234)), U16_MSB(U32_MSW(c1234)),          \
                data, len, NULL, 0)

#define NETCALL_C1234_RV(cmd, unit, c1234, reply, replylen)                \
  fuji_bus_call(FUJI_DEVICEID_NETWORK + (unit) - 1, cmd,                   \
                FUJI_FIELD_C1234,                                          \
                U16_LSB(U32_LSW(c1234)), U16_MSB(U32_LSW(c1234)),          \
                U16_LSB(U32_MSW(c1234)), U16_MSB(U32_MSW(c1234)),          \
                NULL, 0, reply, replylen)

#if defined(_CMOC_VERSION_)

// On CoCo and Adam all network calls *must* have aux1 and aux2
#define NETCALL(cmd, unit)                                                 \
  NETCALL_A1_A2(cmd, unit, 0, 0)

#define NETCALL_D(cmd, unit, data, len)                                    \
  NETCALL_A1_A2_D(cmd, unit, 0, 0, data, len)

#define NETCALL_RV(cmd, unit, reply, replylen)                             \
  NETCALL_A1_A2_RV(cmd, unit, 0, 0, reply, replylen)

#define NETCALL_A1(cmd, unit, a1)                                          \
  NETCALL_A1_A2(cmd, unit, a1, 0)

#define NETCALL_A1_D(cmd, unit, a1, data, len)                             \
  NETCALL_A1_A2_D(cmd, unit, a1, 0, data, len)

#define NETCALL_A1_RV(cmd, unit, a1, reply, replylen)                      \
  NETCALL_A1_A2(cmd, unit, a1, 0, reply, replylen)

#else /* ! _CMOC_VERSION_ */

#define NETCALL(cmd, unit)                                                 \
  fuji_bus_call(FUJI_DEVICEID_NETWORK + (unit) - 1, cmd,                   \
                FUJI_FIELD_NONE,                                           \
                0, 0, 0, 0,                                                \
                NULL, 0, NULL, 0)

#define NETCALL_D(cmd, unit, data, len)                                    \
  fuji_bus_call(FUJI_DEVICEID_NETWORK + (unit) - 1, cmd,                   \
                FUJI_FIELD_NONE,                                           \
                0, 0, 0, 0,                                                \
                data, len, NULL, 0)

#define NETCALL_RV(cmd, unit, reply, replylen)                             \
  fuji_bus_call(FUJI_DEVICEID_NETWORK + (unit) - 1, cmd,                   \
                FUJI_FIELD_NONE,                                           \
		0, 0, 0, 0,                                                \
                NULL, 0, reply, replylen)

#define NETCALL_A1(cmd, unit, a1)                                          \
  fuji_bus_call(FUJI_DEVICEID_NETWORK + (unit) - 1, cmd,                   \
                FUJI_FIELD_A1,                                             \
                a1, 0, 0, 0,                                               \
                NULL, 0, NULL, 0)

#define NETCALL_A1_D(cmd, unit, a1, data, len)                             \
  fuji_bus_call(FUJI_DEVICEID_NETWORK + (unit) - 1, cmd,                   \
                FUJI_FIELD_A1,                                             \
		a1, 0, 0, 0,                                               \
                data, len, NULL, 0)

#define NETCALL_A1_RV(cmd, unit, a1, reply, replylen)                      \
  fuji_bus_call(FUJI_DEVICEID_NETWORK + (unit) - 1, cmd,                   \
                FUJI_FIELD_A1,                                             \
		a1, 0, 0, 0,                                               \
                NULL, 0, reply, replylen)

#endif /* _CMOC_VERSION_ */

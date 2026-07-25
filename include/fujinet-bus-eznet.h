#ifndef FUJINET_BUS_EZNET_H
#define FUJINET_BUS_EZNET_H

#define NETCALL_A1_A2(cmd, unit, a1, a2)                                          \
  DEVCALL_A1_A2(FUJI_DEVICEID_NETWORK + (unit) - 1, cmd, a1, a2)

#define NETCALL_A1_A2_D(cmd, unit, a1, a2, data, len)                             \
  DEVCALL_A1_A2_D(FUJI_DEVICEID_NETWORK + (unit) - 1, cmd, a1, a2, data, len)

#define NETCALL_A1_A2_RV(cmd, unit, a1, a2, reply, replylen)                      \
  DEVCALL_A1_A2_RV(FUJI_DEVICEID_NETWORK + (unit) - 1, cmd, a1, a2, reply, replylen)

#define NETCALL_A1_A2_A3(cmd, unit, a1, a2, a3)                                   \
  DEVCALL_A1_A2_A3(FUJI_DEVICEID_NETWORK + (unit) - 1, cmd, a1, a2, a3)

#define NETCALL_A1_A2_A3_D(cmd, unit, a1, a2, a3, data, len)                      \
  DEVCALL_A1_A2_A3_D(FUJI_DEVICEID_NETWORK + (unit) - 1, cmd, a1, a2, a3, data, len)

#define NETCALL_A1_A2_A3_RV(cmd, unit, a1, a2, a3, reply, replylen)               \
  DEVCALL_A1_A2_A3_RV(FUJI_DEVICEID_NETWORK + (unit) - 1, cmd, a1, a2, a3,        \
                      reply, replylen)

#define NETCALL_A1_A2_A3_A4(cmd, unit, a1, a2, a3, a4)                            \
  DEVCALL_A1_A2_A3_A4(FUJI_DEVICEID_NETWORK + (unit) - 1, cmd, a1, a2, a3, a4)

#define NETCALL_A1_A2_A3_A4_D(cmd, unit, a1, a2, a3, a4, data, len)               \
  DEVCALL_A1_A2_A3_A4_D(FUJI_DEVICEID_NETWORK + (unit) - 1, cmd, a1, a2, a3, a4,  \
                        data, len)

#define NETCALL_A1_A2_A3_A4_RV(cmd, unit, a1, a2, a3, a4, reply, replylen)        \
  DEVCALL_A1_A2_A3_A4_RV(FUJI_DEVICEID_NETWORK + (unit) - 1, cmd, a1, a2, a3, a4, \
                         reply, replylen)

#define NETCALL_B12(cmd, unit, b12)                                               \
  DEVCALL_B12(FUJI_DEVICEID_NETWORK + (unit) - 1, cmd, b12)

#define NETCALL_B12_D(cmd, unit, b12, data, len)                                  \
  DEVCALL_B12_D(FUJI_DEVICEID_NETWORK + (unit) - 1, cmd, b12, data, len)

#define NETCALL_B12_RV(cmd, unit, b12, reply, replylen)                           \
  DEVCALL_B12_RV(FUJI_DEVICEID_NETWORK + (unit) - 1, cmd, b12, reply, replylen)

#define NETCALL_B12_B34(cmd, unit, b12, b34)                                      \
  DEVCALL_B12_B34(FUJI_DEVICEID_NETWORK + (unit) - 1, cmd, b12, b34)

#define NETCALL_B12_B34_D(cmd, unit, b12, b34, data, len)                         \
  DEVCALL_B12_B34_D(FUJI_DEVICEID_NETWORK + (unit) - 1, cmd, b12, b34, data, len)

#define NETCALL_B12_B34_RV(cmd, unit, b12, b34, reply, replylen)                  \
  DEVCALL_B12_B34_RV(FUJI_DEVICEID_NETWORK + (unit) - 1, cmd, b12, b34, reply, replylen)

#define NETCALL_C1234(cmd, unit, c1234)                                           \
  DEVCALL_C1234(FUJI_DEVICEID_NETWORK + (unit) - 1, cmd, c1234)

#define NETCALL_C1234_D(cmd, unit, c1234, data, len)                              \
  DEVCALL_C1234_D(FUJI_DEVICEID_NETWORK + (unit) - 1, cmd, c1234, data, len)

#define NETCALL_C1234_RV(cmd, unit, c1234, reply, replylen)                       \
  DEVCALL_C1234_RV(FUJI_DEVICEID_NETWORK + (unit) - 1, cmd, c1234, reply, replylen)

#if defined(_CMOC_VERSION_)

// On CoCo all network calls *must* have aux1 and aux2
#define NETCALL(cmd, unit)                                                        \
  DEVCALL_A1_A2(FUJI_DEVICEID_NETWORK + (unit) - 1, cmd, 0, 0)

#define NETCALL_D(cmd, unit, data, len)                                           \
  DEVCALL_A1_A2_D(FUJI_DEVICEID_NETWORK + (unit) - 1, cmd, 0, 0, data, len)

#define NETCALL_RV(cmd, unit, reply, replylen)                                    \
  DEVCALL_A1_A2_RV(FUJI_DEVICEID_NETWORK + (unit) - 1, cmd, 0, 0, reply, replylen)

#define NETCALL_A1(cmd, unit, a1)                                                 \
  DEVCALL_A1_A2(FUJI_DEVICEID_NETWORK + (unit) - 1, cmd, a1, 0)

#define NETCALL_A1_D(cmd, unit, a1, data, len)                                    \
  DEVCALL_A1_A2_D(FUJI_DEVICEID_NETWORK + (unit) - 1, cmd, a1, 0, data, len)

#define NETCALL_A1_RV(cmd, unit, a1, reply, replylen)                             \
  DEVCALL_A1_A2_RV(FUJI_DEVICEID_NETWORK + (unit) - 1, cmd, a1, 0, reply, replylen)

#else /* ! _CMOC_VERSION_ */

#define NETCALL(cmd, unit)                                                        \
  DEVCALL(FUJI_DEVICEID_NETWORK + (unit) - 1, cmd)

#define NETCALL_D(cmd, unit, data, len)                                           \
  DEVCALL_D(FUJI_DEVICEID_NETWORK + (unit) - 1, cmd, data, len)

#define NETCALL_RV(cmd, unit, reply, replylen)                                    \
  DEVCALL_RV(FUJI_DEVICEID_NETWORK + (unit) - 1, cmd, reply, replylen)

#define NETCALL_A1(cmd, unit, a1)                                                 \
  DEVCALL_A1(FUJI_DEVICEID_NETWORK + (unit) - 1, cmd, a1)

#define NETCALL_A1_D(cmd, unit, a1, data, len)                                    \
  DEVCALL_A1_D(FUJI_DEVICEID_NETWORK + (unit) - 1, cmd, a1, data, len)

#define NETCALL_A1_RV(cmd, unit, a1, reply, replylen)                             \
  DEVCALL_A1_RV(FUJI_DEVICEID_NETWORK + (unit) - 1, cmd, a1, reply, replylen)

#endif /* _CMOC_VERSION_ */

#endif /* FUJINET_BUS_EZNET_H */

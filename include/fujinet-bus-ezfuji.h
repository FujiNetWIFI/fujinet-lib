#ifndef FUJINET_BUS_EZFUJI_H
#define FUJINET_BUS_EZFUJI_H

#include <fujinet-bus-ezcall.h>

#define FUJICALL(cmd)                                                 \
  DEVCALL(FUJI_DEVICEID_FUJINET, cmd)

#define FUJICALL_D(cmd, data, len)                                    \
  DEVCALL_D(FUJI_DEVICEID_FUJINET, cmd, data, len)

#define FUJICALL_RV(cmd, reply, replylen)                             \
  DEVCALL_RV(FUJI_DEVICEID_FUJINET, cmd, reply, replylen)

#define FUJICALL_A1(cmd, a1)                                          \
  DEVCALL_A1(FUJI_DEVICEID_FUJINET, cmd, a1)

#define FUJICALL_A1_D(cmd, a1, data, len)                             \
  DEVCALL_A1_D(FUJI_DEVICEID_FUJINET, cmd, a1, data, len)

#define FUJICALL_A1_RV(cmd, a1, reply, replylen)                      \
  DEVCALL_A1_RV(FUJI_DEVICEID_FUJINET, cmd, a1, reply, replylen)

#define FUJICALL_A1_A2(cmd, a1, a2)                                   \
  DEVCALL_A1_A2(FUJI_DEVICEID_FUJINET, cmd, a1, a2)

#define FUJICALL_A1_A2_D(cmd, a1, a2, data, len)                      \
  DEVCALL_A1_A2_D(FUJI_DEVICEID_FUJINET, cmd, a1, a2, data, len)

#define FUJICALL_A1_A2_RV(cmd, a1, a2, reply, replylen)               \
  DEVCALL_A1_A2_RV(FUJI_DEVICEID_FUJINET, cmd, a1, a2, reply, replylen)

#define FUJICALL_A1_A2_A3(cmd, a1, a2, a3)                            \
  DEVCALL_A1_A2_A3(FUJI_DEVICEID_FUJINET, cmd, a1, a2, a3)

#define FUJICALL_A1_A2_A3_D(cmd, a1, a2, a3, data, len)               \
  DEVCALL_A1_A2_A3_D(FUJI_DEVICEID_FUJINET, cmd, a1, a2, a3, data, len)

#define FUJICALL_A1_A2_A3_RV(cmd, a1, a2, a3, reply, replylen)        \
  DEVCALL_A1_A2_A3_RV(FUJI_DEVICEID_FUJINET, cmd, a1, a2, a3, reply, replylen)

#define FUJICALL_A1_A2_A3_A4(cmd, a1, a2, a3, a4)                     \
  DEVCALL_A1_A2_A3_A4(FUJI_DEVICEID_FUJINET, cmd, a1, a2, a3, a4)

#define FUJICALL_A1_A2_A3_A4_D(cmd, a1, a2, a3, a4, data, len)        \
  DEVCALL_A1_A2_A3_A4_D(FUJI_DEVICEID_FUJINET, cmd, a1, a2, a3, a4, data, len)

#define FUJICALL_A1_A2_A3_A4_RV(cmd, a1, a2, a3, a4, reply, replylen) \
  DEVCALL_A1_A2_A3_A4_RV(FUJI_DEVICEID_FUJINET, cmd, a1, a2, a3, a4, reply, replylen)

#define FUJICALL_B12(cmd, b12)                                        \
  DEVCALL_B12(FUJI_DEVICEID_FUJINET, cmd, b12)

#define FUJICALL_B12_D(cmd, b12, data, len)                           \
  DEVCALL_B12_D(FUJI_DEVICEID_FUJINET, cmd, b12, data, len)

#define FUJICALL_B12_RV(cmd, b12, reply, replylen)                    \
  DEVCALL_B12_RV(FUJI_DEVICEID_FUJINET, cmd, b12, reply, replylen)

#define FUJICALL_B12_B34(cmd, b12, b34)                               \
  DEVCALL_B12_B34(FUJI_DEVICEID_FUJINET, cmd, b12, b34)

#define FUJICALL_B12_B34_D(cmd, b12, b34, data, len)                  \
  DEVCALL_B12_B34_D(FUJI_DEVICEID_FUJINET, cmd, b12, b34, data, len)

#define FUJICALL_B12_B34_RV(cmd, b12, b34, reply, replylen)           \
  DEVCALL_B12_B34_RV(FUJI_DEVICEID_FUJINET, cmd, b12, b34, reply, replylen)

#define FUJICALL_C1234(cmd, c1234)                                    \
  DEVCALL_C1234(FUJI_DEVICEID_FUJINET, cmd, c1234)

#define FUJICALL_C1234_D(cmd, c1234, data, len)                       \
  DEVCALL_C1234_D(FUJI_DEVICEID_FUJINET, cmd, c1234, data, len)

#define FUJICALL_C1234_RV(cmd, c1234, reply, replylen)                \
  DEVCALL_C1234_RV(FUJI_DEVICEID_FUJINET, cmd, c1234, reply, replylen)

#endif /* FUJINET_BUS_EZFUJI_H */

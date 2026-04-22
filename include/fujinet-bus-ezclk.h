#ifndef FUJINET_BUS_EZCLK_H
#define FUJINET_BUS_EZCLK_H

#include <fujinet-endian.h>
#include <fujinet-bus.h>

#define CLKCALL(cmd)                                                    \
  fuji_bus_call(FUJI_DEVICEID_CLOCK, cmd,                               \
                FUJI_FIELD_NONE,                                        \
                0, 0, 0, 0,                                             \
                NULL, 0, NULL, 0)

#define CLKCALL_D(cmd, data, len)                                       \
  fuji_bus_call(FUJI_DEVICEID_CLOCK, cmd,                               \
                FUJI_FIELD_NONE,                                        \
                0, 0, 0, 0,                                             \
                data, len, NULL, 0)

#define CLKCALL_RV(cmd, reply, replylen)                                \
  fuji_bus_call(FUJI_DEVICEID_CLOCK, cmd,                               \
                FUJI_FIELD_NONE,                                        \
                0, 0, 0, 0,                                             \
                NULL, 0, reply, replylen)

#define CLKCALL_A1_RV(cmd, a1, reply, replylen)                         \
  fuji_bus_call(FUJI_DEVICEID_CLOCK, cmd,                               \
                FUJI_FIELD_A1,                                          \
                a1, 0, 0, 0,                                            \
                NULL, 0, reply, replylen)

#define CLKCALL_B12_D(cmd, b12, data, len)                              \
  fuji_bus_call(FUJI_DEVICEID_CLOCK, cmd,                               \
                FUJI_FIELD_B12,                                         \
                U16_LSB(b12), U16_MSB(b12), 0, 0,                       \
                data, len, NULL, 0)

#endif /* FUJINET_BUS_EZCLK_H */

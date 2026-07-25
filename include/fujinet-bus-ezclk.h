#ifndef FUJINET_BUS_EZCLK_H
#define FUJINET_BUS_EZCLK_H

#define CLKCALL(cmd)                            \
  DEVCALL(FUJI_DEVICEID_CLOCK, cmd)

#define CLKCALL_D(cmd, data, len)               \
  DEVCALL_D(FUJI_DEVICEID_CLOCK, cmd, data, len)

#define CLKCALL_RV(cmd, reply, replylen)        \
  DEVCALL_RV(FUJI_DEVICEID_CLOCK, cmd, reply, replylen)

#define CLKCALL_A1_RV(cmd, a1, reply, replylen) \
  DEVCALL_A1_RV(FUJI_DEVICEID_CLOCK, cmd, a1, reply, replylen)

#define CLKCALL_B12_D(cmd, b12, data, len)      \
  DEVCALL_A1_A2_D(FUJI_DEVICEID_CLOCK, cmd, U16_LSB(b12), U16_MSB(b12), data, len)

#endif /* FUJINET_BUS_EZCLK_H */

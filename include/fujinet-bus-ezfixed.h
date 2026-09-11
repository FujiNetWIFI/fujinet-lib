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
  fuji_bus_call(dev, cmd, FUJI_FIELD_C1234, NATIVE_SPLIT_U32(c1234), NULL, 0)

#define DEVCALL_C1234_D(dev, cmd, c1234, data, len)                                         \
  fuji_bus_call(dev, cmd, FUJI_FIELD_C1234 | FUJI_FIELD_DATA,                               \
                NATIVE_SPLIT_U32(c1234), data, len)

#define DEVCALL_C1234_RV(dev, cmd, c1234, reply, replylen)                                  \
  fuji_bus_call(dev, cmd, FUJI_FIELD_C1234 | FUJI_FIELD_REPLY,                              \
                NATIVE_SPLIT_U32(c1234), reply, replylen)

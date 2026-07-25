#ifndef FB_HEADER_H
#define FB_HEADER_H

#include <fujinet-int.h>

typedef struct {
  uint8_t opcode;
  union {
    uint8_t cmd;
    struct {
      uint8_t unit;
      uint8_t cmd;
    } net;
  } fn;
} fujibus_header;

#endif /* FB_HEADER_H */

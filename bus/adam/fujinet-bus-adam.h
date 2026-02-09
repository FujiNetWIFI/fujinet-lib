#ifndef FUJINET_BUS_ADAM_H
#define FUJINET_BUS_ADAM_H

#include <stdint.h>

enum {
  DCB_COMMAND_IDLE      = 0x00,
  DCB_COMMAND_STATUS    = 0x01,
  DCB_COMMAND_RESET     = 0x02,
  DCB_COMMAND_WRITE     = 0x03,
  DCB_COMMAND_READ      = 0x04,

  DCB_STATUS_FINISH     = 0x80,
  DCB_STATUS_KBD_NAK    = 0x8C,
  DCB_STATUS_PR_NAK     = 0x86,
  DCB_STATUS_ETX        = 0x03,
  DCB_STATUS_TIMEOUT    = 0x9B,
  DCB_STATUS_UNREADABLE = 0xE1,
};

enum {
  ADAM_ID_FUJINET  = 0x0F,

  ADAM_ID_KEYBOARD = 0x01,
  ADAM_ID_PRINTER  = 0x02,
  ADAM_ID_DISK     = 0x04,
  ADAM_ID_DISK2    = 0x05,
  ADAM_ID_DISK3    = 0x06,
  ADAM_ID_DISK4    = 0x07,
  ADAM_ID_TAPE     = 0x08,
  ADAM_ID_NETWORK  = 0x09,
};

typedef struct
{
  uint8_t status;
  void *buf;
  uint16_t len;
  uint32_t block;
  uint8_t unit;
  uint8_t reserved0;
  uint8_t reserved1;
  uint8_t reserved2;
  uint8_t reserved3;
  uint8_t reserved4;
  uint8_t reserved5;
  uint8_t dev;
  uint16_t max_len;
  uint8_t type;
  uint8_t dev_status;
} DCB;

extern DCB *dcb_find(uint8_t device);
extern uint8_t dcb_io(DCB *dcb, uint8_t mode, void *buffer, size_t length);
extern uint8_t fuji_remap_device(uint8_t device);

#endif /* FUJINET_BUS_ADAM_H */

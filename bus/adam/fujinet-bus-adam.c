#include "fujinet-bus.h"
#include <string.h>

#define MAX_ADAM_PACKET 512
#define MAX_RETRIES 20

#define DCB_COUNT_ADDR ((uint8_t *) 0xFEC3)
#define DCB_TABLE_ADDR ((DCB *) 0xFEC4)

#define DCB_COMPLETE_MASK 0x80

enum {
  DCB_COMMAND_IDLE   = 0x00,
  DCB_COMMAND_STATUS = 0x01,
  DCB_COMMAND_RESET  = 0x02,
  DCB_COMMAND_WRITE  = 0x03,
  DCB_COMMAND_READ   = 0x04,

  DCB_STATUS_FINISH  = 0x80,
  DCB_STATUS_KBD_NAK = 0x8C,
  DCB_STATUS_PR_NAK  = 0x86,
  DCB_STATUS_ETX     = 0x03,
  DCB_STATUS_TIMEOUT = 0x9B,
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

static uint8_t fb_packet[MAX_ADAM_PACKET];

DCB *dcb_find(uint8_t device)
{
  uint8_t idx;
  uint8_t num_dcbs = *DCB_COUNT_ADDR;
  DCB *dcb_table = DCB_TABLE_ADDR;


  for (idx = 0; idx < num_dcbs; idx++) {
    if ((dcb_table[idx].dev & 0x0F) == device)
	return &dcb_table[idx];
  }

  // Device not found, return NULL.
  return NULL;
}

uint8_t dcb_io(DCB *dcb, uint8_t mode, void *buffer, size_t length)
{
  uint_fast8_t count;


  for (count = 0; count < MAX_RETRIES; count++) {
    dcb->len = length;
    dcb->buf = buffer;
    dcb->status = mode;

    while (dcb->status < DCB_COMPLETE_MASK)
      ;

    if (dcb->status != DCB_STATUS_TIMEOUT)
      break;
  }

  return dcb->status;
}

bool fuji_bus_call(uint8_t device, uint8_t fuji_cmd, uint8_t fields,
		   uint8_t aux1, uint8_t aux2, uint8_t aux3, uint8_t aux4,
		   const void *data, size_t data_length,
		   void *reply, size_t reply_length)
{
  DCB *dcb;
  uint16_t idx, numbytes;
  uint8_t status;


  if (device > 0x0F) {
    // Device ID is standard FujiNet device ID, remap it to Adam
    if (device == FUJI_DEVICEID_FUJINET)
      device = ADAM_ID_FUJINET;
    else if (device == FUJI_DEVICEID_PRINTER)
      device = ADAM_ID_PRINTER;
    else if (device >= FUJI_DEVICEID_DISK && device <= FUJI_DEVICEID_DISK_LAST)
      device = device - FUJI_DEVICEID_DISK + ADAM_ID_DISK;
    else if (device >= FUJI_DEVICEID_NETWORK && device <= FUJI_DEVICEID_NETWORK_LAST) {
      device = device - FUJI_DEVICEID_NETWORK + ADAM_ID_NETWORK;
      // Adam currently only supports 2 network devices
      if (device >= ADAM_ID_NETWORK + 2)
        return false;
    }
    else {
      // No remap possible
      return false;
    }
  }

  idx = 0;
  fb_packet[idx++] = fuji_cmd;

  numbytes = fuji_field_numbytes(fields);
  if (numbytes) {
    fb_packet[idx++] = aux1;
    numbytes--;
  }
  if (numbytes) {
    fb_packet[idx++] = aux2;
    numbytes--;
  }
  if (numbytes) {
    fb_packet[idx++] = aux3;
    numbytes--;
  }
  if (numbytes) {
    fb_packet[idx++] = aux4;
    numbytes--;
  }
  if (data) {
    memcpy(&fb_packet[idx], data, data_length);
    idx += data_length;
  }

  dcb = dcb_find(device);
  if (!dcb)
    return false;

  status = dcb_io(dcb, DCB_COMMAND_WRITE, fb_packet, idx);
  if (status != DCB_STATUS_FINISH)
    return false;

  if (reply) {
    status = dcb_io(dcb, DCB_COMMAND_READ, reply, reply_length);
    if (status != DCB_STATUS_FINISH)
      return false;
  }

  return true;
}

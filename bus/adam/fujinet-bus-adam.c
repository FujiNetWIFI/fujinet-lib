#include "fujinet-bus-adam.h"
#include "fujinet-bus.h"
#include "fujinet-commands.h"
#include <string.h>

#include <stdio.h> // debug

#define MAX_ADAM_PACKET 512
#define MAX_RETRIES 20

#define DCB_COUNT_ADDR ((uint8_t *) 0xFEC3)
#define DCB_TABLE_ADDR ((DCB *) 0xFEC4)

#define DCB_COMPLETE_MASK 0x80

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

uint8_t fuji_remap_device(uint8_t device)
{
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
        return 0;
    }
    else {
      // No remap possible
      return 0;
    }
  }

  return device;
}

bool fuji_bus_call(uint8_t device, uint8_t fuji_cmd, uint8_t fields,
		   uint8_t aux1, uint8_t aux2, uint8_t aux3, uint8_t aux4,
		   const void *data, size_t data_length,
		   void *reply, size_t reply_length)
{
  DCB *dcb;
  uint16_t idx, numbytes;
  uint8_t status;


  device = fuji_remap_device(device);
  if (!device)
    return false;

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

#if 0
size_t fuji_bus_read(uint8_t device, void *buffer, size_t length)
{
  uint8_t status;
  DCB *dcb;


  device = fuji_remap_device(device);
  dcb = dcb_find(device);
  status = dcb_io(dcb, DCB_COMMAND_READ, buffer, length);
  printf("DCB STATUS %02x len=%d\n", status, dcb->len);
  if (status != DCB_STATUS_FINISH)
    dcb->len = 0;
  return dcb->len;
}
#endif

size_t fuji_bus_write(uint8_t device, const void *buffer, size_t length)
{
  NETCALL_D(FUJICMD_WRITE, device - FUJI_DEVICEID_NETWORK + 1, buffer, length);
  return length;
}

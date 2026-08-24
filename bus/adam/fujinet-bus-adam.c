#include "fujinet-bus-adam.h"
#include "fujinet-bus.h"
#include "fujinet-commands.h"
#include "fujinet-const.h"
#include <string.h>

#include <stdio.h> // debug

#define MAX_ADAM_PACKET 512

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

uint8_t dcb_io(DCB *dcb, uint8_t mode, void *buffer, size_t length,
	       uint_fast16_t retries)
{
  uint_fast16_t count;


  for (count = 0; count < retries; count++) {
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
    else if (device == FUJI_DEVICEID_CLOCK)
      device = ADAM_ID_CLOCK;
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

/* AdamNet completes a write on delivery, not on completion. */
uint8_t bus_ready(DCB *dcb, uint_fast16_t retries)
{
  while (retries--)
    if (dcb_io(dcb, DCB_COMMAND_STATUS, NULL, 0, 1) == DCB_STATUS_FINISH)
      return true;

  return false;
}

bool fuji_bus_call(uint8_t device, uint8_t fuji_cmd, uint8_t fields,
		   uint8_t aux1, uint8_t aux2, uint8_t aux3, uint8_t aux4,
		   const void *buf, size_t buf_length)
{
  DCB *dcb;
  uint16_t idx, numbytes, retries;
  uint8_t status;


  device = fuji_remap_device(device);
  if (!device)
    return false;

  idx = 0;
  fb_packet[idx++] = fuji_cmd;

  numbytes = fuji_field_numbytes(fields);
  if (numbytes > 0)
    fb_packet[idx++] = aux1;
  if (numbytes > 1)
    fb_packet[idx++] = aux2;
  if (numbytes > 2)
    fb_packet[idx++] = aux3;
  if (numbytes > 3)
    fb_packet[idx++] = aux4;
  if (fields & FUJI_FIELD_DATA) {
    memcpy(&fb_packet[idx], buf, buf_length);
    idx += buf_length;
  }

  dcb = dcb_find(device);
  if (!dcb)
    return false;

  status = dcb_io(dcb, DCB_COMMAND_WRITE, fb_packet, idx, MAX_RETRIES);
  if (status != DCB_STATUS_FINISH)
    return false;

  if (fields & FUJI_FIELD_REPLY) {
    retries = MAX_RETRIES;
    if (fuji_cmd == FUJICMD_COPY_FILE
        || fuji_cmd == FUJICMD_SCAN_NETWORKS)
      retries = COPY_RETRIES;
    status = dcb_io(dcb, DCB_COMMAND_READ, (void *) buf, buf_length, retries);
    if (status != DCB_STATUS_FINISH)
      return false;
  }

  return true;
}

#if 0
size_t network_bus_read(uint8_t device, void *buffer, size_t length)
{
  uint8_t status;
  DCB *dcb;


  device = fuji_remap_device(device);
  dcb = dcb_find(device);
  status = dcb_io(dcb, DCB_COMMAND_READ, buffer, length, MAX_RETRIES);
  printf("DCB STATUS %02x len=%d\n", status, dcb->len);
  if (status != DCB_STATUS_FINISH)
    dcb->len = 0;
  return dcb->len;
}
#endif

size_t network_bus_write(uint8_t device, const void *buffer, size_t length)
{
  NETCALL_D(FUJICMD_WRITE, device - FUJI_DEVICEID_NETWORK + 1, buffer, length);
  return length;
}

/*
  appkeys are variable length strings. Because SmartPort supports
  variable length data packets, aux1/aux2 aren't used to send the
  length of the string. Instead only the string data is sent with no
  length field, no block size, no padding.
*/

bool fuji_bus_appkey_read(void *string, uint16_t *length)
{
  DCB *dcb;
  uint8_t status, device;


  device = fuji_remap_device(FUJI_DEVICEID_FUJINET);
  if (!device)
    return false;

  dcb = dcb_find(device);
  if (!dcb)
    return false;

  fb_packet[0] = FUJICMD_READ_APPKEY;
  status = dcb_io(dcb, DCB_COMMAND_WRITE, fb_packet, 1, MAX_RETRIES);
  if (status != DCB_STATUS_FINISH)
    return false;

  status = dcb_io(dcb, DCB_COMMAND_READ, string, MAX_APPKEY_LEN, MAX_RETRIES);
  if (status != DCB_STATUS_FINISH)
    return false;

  *length = dcb->len;
  return true;
}

bool fuji_bus_appkey_write(void *string, uint16_t length)
{
  return FUJICALL_D(FUJICMD_WRITE_APPKEY, string, length);
}

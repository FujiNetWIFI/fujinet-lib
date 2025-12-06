#include "fujinet-bus.h"
#include "fujinet-fuji-coco.h"
#include "fujinet-commands.h"
#include "fujinet-const.h"
#include "fujinet-network-coco.h"
#include "dw.h"

typedef struct {
  uint16_t length;
  uint8_t data[MAX_APPKEY_LEN];
} FNAppKeyString;

typedef struct {
  uint8_t opcode;
  uint8_t cmd;
} fujibus_header;

typedef struct {
  byte opcode;
  byte unit;
  byte cmd;
} fujinw_header;

static fujibus_header fb_header;
static fujinw_header nw_header;
static FNAppKeyString appkey_buf;
static unsigned char buffer[256];

bool fuji_net_call(uint8_t unit, uint8_t fuji_cmd, uint8_t fields,
		   uint8_t aux1, uint8_t aux2, uint8_t aux3, uint8_t aux4,
		   const void *data, size_t data_length,
		   void *reply, size_t reply_length)
{
  uint8_t err;
  uint16_t idx, numbytes;


  nw_header.opcode = OP_NET;
  nw_header.unit = unit;
  nw_header.cmd = fuji_cmd;

  idx = 0;
  numbytes = fuji_field_numbytes(fields);
  if (numbytes) {
    buffer[idx++] = aux1;
    numbytes--;
  }
  if (numbytes) {
    buffer[idx++] = aux2;
    numbytes--;
  }
  if (numbytes) {
    buffer[idx++] = aux3;
    numbytes--;
  }
  if (numbytes) {
    buffer[idx++] = aux4;
    numbytes--;
  }
  if (data) {
    memcpy(&buffer[idx], data, data_length);
    idx += data_length;
  }

  bus_ready();
  dwwrite((unsigned char *) &nw_header, sizeof(nw_header));
  if (idx)
    dwwrite(buffer, idx);

  err = network_get_error(unit);
  if (err)
    return false;

  if (reply) {
    err = network_get_response(unit, (unsigned char *) reply, reply_length);
    return !err;
  }

  return true;
}

bool fuji_bus_call(uint8_t device, uint8_t fuji_cmd, uint8_t fields,
		   uint8_t aux1, uint8_t aux2, uint8_t aux3, uint8_t aux4,
		   const void *data, size_t data_length,
		   void *reply, size_t reply_length)
{
  uint16_t idx, numbytes;


  if (device >= FUJI_DEVICEID_NETWORK  && device <= FUJI_DEVICEID_NETWORK_LAST)
    return fuji_net_call(device - FUJI_DEVICEID_NETWORK + 1, fuji_cmd, fields,
                         aux1, aux2, aux3, aux4, data, data_length, reply, reply_length);

  if (device != FUJI_DEVICEID_FUJINET)
    return false;

  fb_header.opcode = OP_FUJI;
  fb_header.cmd = fuji_cmd;

  idx = 0;
  numbytes = fuji_field_numbytes(fields);
  if (numbytes) {
    buffer[idx++] = aux1;
    numbytes--;
  }
  if (numbytes) {
    buffer[idx++] = aux2;
    numbytes--;
  }
  if (numbytes) {
    buffer[idx++] = aux3;
    numbytes--;
  }
  if (numbytes) {
    buffer[idx++] = aux4;
    numbytes--;
  }
  if (data) {
    memcpy(&buffer[idx], data, data_length);
    idx += data_length;
  }

  bus_ready();
  dwwrite((unsigned char *) &fb_header, sizeof(fb_header));
  if (idx)
    dwwrite(buffer, idx);

  if (fuji_get_error())
    return false;

  if (reply)
    return (bool) fuji_get_response((unsigned char *) reply, reply_length);

  return true;
}

uint16_t fuji_bus_read(uint8_t device, void *buffer, size_t length)
{
  nw_header.opcode = OP_NET;
  nw_header.unit = device - FUJI_DEVICEID_NETWORK + 1;
  nw_header.cmd = FUJICMD_READ;

  bus_ready();
  dwwrite((uint8_t *) &nw_header, sizeof(nw_header));
  dwwrite((uint8_t *) &length, sizeof(length));
  network_get_response(nw_header.unit, (uint8_t *) buffer, length);

  return length;
}

uint16_t fuji_bus_write(uint8_t device, const void *buffer, size_t length)
{
  nw_header.opcode = OP_NET;
  nw_header.unit = device - FUJI_DEVICEID_NETWORK + 1;
  nw_header.cmd = FUJICMD_WRITE;

  bus_ready();
  dwwrite((uint8_t *) &nw_header, sizeof(nw_header));
  dwwrite((uint8_t *) &length, sizeof(length));
  dwwrite((uint8_t *) buffer, length);

  return network_get_error(nw_header.unit);
}

/*
  appkeys are variable length strings. CoCo drivewire is serial but
  drivewireFuji in the FujiNet firmware seems to be using fixed length
  packets with a header to indicate the true length of the key, same
  as Atari SIO. On write the aux1/aux2 fields are combined into a
  uint16_t. On read, there is an extra 2 bytes of header to indicate
  how much of the fixed block represents the string.
*/

bool fuji_bus_appkey_read(void *string, uint16_t *length)
{
 // Caller may not have room for length header so use our own buffer to read
  if (!FUJICALL_RV(FUJICMD_READ_APPKEY, &appkey_buf, sizeof(appkey_buf)))
    return false;
  *length = appkey_buf.length;
  memmove(string, appkey_buf.data, appkey_buf.length);
  return true;
}

bool fuji_bus_appkey_write(void *string, uint16_t length)
{
  return FUJICALL_B12_D(FUJICMD_WRITE_APPKEY, length, string, MAX_APPKEY_LEN);
}

#include "fujinet-bus.h"
#include "fujinet-fuji-coco.h"
#include "fujinet-commands.h"
#include "fujinet-const.h"
#include "dw.h"

typedef struct {
  uint16_t length;
  uint8_t data[MAX_APPKEY_LEN];
} FNAppKeyString;

typedef struct {
  uint8_t opcode;
  uint8_t cmd;
  uint8_t data[256];
} fujibus_packet;

fujibus_packet fb_packet;
FNAppKeyString appkey_buf;

bool fuji_bus_call(uint8_t device, uint8_t fuji_cmd, uint8_t fields,
		   uint8_t aux1, uint8_t aux2, uint8_t aux3, uint8_t aux4,
		   const void *data, size_t data_length,
		   void *reply, size_t reply_length)
{
  uint16_t idx = 0;


  fb_packet.opcode = OP_FUJI;
  fb_packet.cmd = fuji_cmd;

  if (fields & FUJI_FIELD_AUX1)
    fb_packet.data[idx++] = aux1;
  if (fields & FUJI_FIELD_AUX2)
    fb_packet.data[idx++] = aux2;
  if (fields & FUJI_FIELD_AUX3)
    fb_packet.data[idx++] = aux3;
  if (fields & FUJI_FIELD_AUX4)
    fb_packet.data[idx++] = aux4;
  if (data) {
    memcpy(&fb_packet.data[idx], data, data_length);
    idx += data_length;
  }

  bus_ready();
  dwwrite((unsigned char *) &fb_packet, 2 + idx);
  if (fuji_get_error())
    return false;

  if (reply)
    return (bool) fuji_get_response((unsigned char *) reply, reply_length);

  return true;
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

#include "fujinet-bus-msdos.h"
#include "fujinet-commands.h"
#include "portio.h"

#include <stdio.h> // debug

enum {
  PACKET_COMPLETE = 'C',
  PACKET_ERROR = 'E',
};

#define FUJI_DIR_NONE    0x00
#define FUJI_DIR_READ    0x40
#define FUJI_DIR_WRITE   0x80

extern int fujiF5w(uint16_t descrdir, uint16_t devcom,
                  uint16_t aux12, uint16_t aux34, void far *buffer, uint16_t length);
#pragma aux fujiF5w = \
  "int 0xf5" \
  parm [dx] [ax] [cx] [si] [es bx] [di] \
  modify [ax]
#define fujiF5(dir, dev, cmd, descr, a12, a34, buf, len)         \
  fujiF5w(descr << 8 | dir, cmd << 8 | dev, a12, a34, buf, len)

bool fuji_bus_call(uint8_t device, uint8_t fuji_cmd, uint8_t fields,
		   uint8_t aux1, uint8_t aux2, uint8_t aux3, uint8_t aux4,
		   const void *data, size_t data_length,
		   void *reply, size_t reply_length)
{
  int rcode;;
  uint8_t direction;
  void far *buffer = NULL;
  size_t length = 0;


  if (data) {
    direction = FUJI_DIR_WRITE;
    buffer = data;
    length = data_length;
  }
  else if (reply) {
    direction = FUJI_DIR_READ;
    buffer = reply;
    length = reply_length;
  }
  else
    direction = FUJI_DIR_NONE;

  rcode = fujiF5(direction, device, fuji_cmd, fields,
                 (aux2 << 8) | aux1,
                 (aux4 << 8) | aux3,
                 buffer, length);

  return rcode == PACKET_COMPLETE;
}

size_t fuji_bus_read(uint8_t device, void *buffer, size_t length)
{
  NETCALL_B12_RV(FUJICMD_READ, device - FUJI_DEVICEID_NETWORK + 1, length, buffer, length);
  return length;
}

size_t fuji_bus_write(uint8_t device, const void *buffer, size_t length)
{
  NETCALL_B12_D(FUJICMD_WRITE, device - FUJI_DEVICEID_NETWORK + 1, length, buffer, length);
  return length;
}

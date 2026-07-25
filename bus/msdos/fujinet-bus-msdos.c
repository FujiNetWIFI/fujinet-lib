#include "fujinet-bus-msdos.h"
#include "fujinet-commands.h"
#include "portio.h"

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
  fujiF5w((descr) << 8 | (dir), (cmd) << 8 | (dev), a12, a34, buf, len)

bool fuji_bus_call(uint8_t device, uint8_t fuji_cmd, uint8_t fields,
		   uint8_t aux1, uint8_t aux2, uint8_t aux3, uint8_t aux4,
		   const void *buf, size_t buf_length)
{
  int rcode;
  uint8_t direction = FUJI_DIR_NONE;


  if (fields & (FUJI_FIELD_DATA | FUJI_FIELD_REPLY))
    direction = fields & FUJI_FIELD_DATA ? FUJI_DIR_WRITE : FUJI_DIR_READ;

  rcode = fujiF5(direction, device, fuji_cmd, fields & 0x07,
                 (aux2 << 8) | aux1,
                 (aux4 << 8) | aux3,
                 (void *) buf, buf_length);

  return rcode == PACKET_COMPLETE;
}

size_t network_bus_read(uint8_t device, void *buffer, size_t length)
{
  NETCALL_B12_RV(FUJICMD_READ, device - FUJI_DEVICEID_NETWORK + 1, length, buffer, length);
  return length;
}

size_t network_bus_write(uint8_t device, const void *buffer, size_t length)
{
  NETCALL_B12_D(FUJICMD_WRITE, device - FUJI_DEVICEID_NETWORK + 1, length, buffer, length);
  return length;
}

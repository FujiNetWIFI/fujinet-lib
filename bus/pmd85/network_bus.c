#include "fb_header.h"
#include "fujinet-fuji-pmd85.h"
#include "fujinet-network-pmd85.h"
#include "dw.h"

#include <fujinet-int.h>
#include <fujinet-bus.h>
#include <fujinet-commands.h>

uint16_t network_bus_read(uint8_t device, void *buffer, size_t length)
{
  fujibus_header fb_header;


  fb_header.opcode = OP_NET;
  fb_header.fn.net.unit = device - FUJI_DEVICEID_NETWORK + 1;
  fb_header.fn.net.cmd = NETCMD_READ;
  uint16_t len = ((uint16_t)length << 8) | ((uint16_t)length >> 8);

  bus_ready();
  dwwrite((uint8_t *) &fb_header, sizeof(fb_header));
  dwwrite((uint8_t *) &len, sizeof(len));
  network_get_response(fb_header.fn.net.unit, (uint8_t *) buffer, length);

  return length;
}

uint16_t network_bus_write(uint8_t device, const void *buffer, size_t length)
{
  fujibus_header fb_header;


  fb_header.opcode = OP_NET;
  fb_header.fn.net.unit = device - FUJI_DEVICEID_NETWORK + 1;
  fb_header.fn.net.cmd = NETCMD_WRITE;
  uint16_t len = ((uint16_t)length << 8) | ((uint16_t)length >> 8);

  bus_ready();
  dwwrite((uint8_t *) &fb_header, sizeof(fb_header));
  dwwrite((uint8_t *) &len, sizeof(len));
  dwwrite((uint8_t *) buffer, length);

  if (network_get_error(fb_header.fn.net.unit))
    length = 0;
  return length;
}

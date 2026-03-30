#include "fujinet-fuji.h"

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>



uint16_t fuji_bus_read(uint8_t device, void *buffer, size_t length)
{
  NETCALL_RV(FUJICMD_READ, device, buffer, length);
  return length;
}

#include "fujinet-fuji.h"

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>


uint16_t fuji_bus_write(uint8_t device, const void *buffer, size_t length)
{
  NETCALL_D(FUJICMD_WRITE, device, buffer, length);
  return length;
}
#include <stdbool.h>
#include <stdint.h>
#include "fujinet-network.h"

uint8_t network_http_set_channel_mode(const char *devicespec, uint8_t mode)
{
  uint8_t device = network_unit(devicespec) + 0x70;
  return int_f5(device,'M',0x00,mode) == 'C' ? FN_ERR_OK : FN_ERR_IO_ERROR;
}

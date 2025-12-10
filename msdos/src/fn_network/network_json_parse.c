#include <stdbool.h>
#include <stdint.h>
#include <fujinet-network.h>
#include <fujinet-fuji-msdos.h>

uint8_t network_json_parse(const char *devicespec)
{
  uint8_t device = network_unit(devicespec) + 0x70;
  uint8_t ret = 0;

  // Set channel mode
  ret = int_f5(device,0xFC,0x01,0x00);
  if (ret != 'C')
    return ret;

  // Ask FujiNet to parse JSON.
  ret = int_f5(device,'P',0x00,0x00);

  return ret == 'C' ? FN_ERR_OK : FN_ERR_IO_ERROR;
}

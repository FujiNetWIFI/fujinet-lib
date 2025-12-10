#include <stdbool.h>
#include <stdint.h>
#include <fujinet-network.h>
#include <fujinet-fuji-msdos.h>

uint8_t network_write(const char* devicespec, const uint8_t *buf, uint16_t len)
{
  uint8_t device = network_unit(devicespec) + 0x70;

  return int_f5_write(device,'W',len&0xFF,len>>8,(void *)buf,len) == 'C';
}

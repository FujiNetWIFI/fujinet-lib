#include <stdbool.h>
#include <stdint.h>
#include <fujinet-network.h>
#include <fujinet-fuji-msdos.h>

uint8_t network_open(const char* devicespec, uint8_t mode, uint8_t trans)
{
  uint8_t device = network_unit(devicespec) + 0x70;
  
  return int_f5_write(device,'O',mode,trans,devicespec,256);
}

#include <stdbool.h>
#include <stdint.h>
#include <fujinet-network.h>
#include <fujinet-fuji-msdos.h>

uint8_t network_close(const char* devicespec)
{
  uint8_t device = network_unit(devicespec) + 0x70;

  return int_f5(device,'C',0x00,0x00) == 'C';
}

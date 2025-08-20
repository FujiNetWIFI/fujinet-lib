#include <stdbool.h>
#include <stdint.h>
#include <fujinet-network.h>
#include <fujinet-fuji-msdos.h>

int network_read_msdos(char* devicespec, byte *buf, unsigned int len)
{
  uint8_t device = network_unit(devicespec) + 0x70;

  return intf5_read(device,'R',len&0xFF,len>>8,(void *)buf,len) == 'C';
}

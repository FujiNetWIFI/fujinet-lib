#include <stdbool.h>
#include <stdint.h>
#include <fujinet-network.h>
#include <fujinet-fuji-msdos.h>

int network_read_msdos(char* devicespec, unsigned char *buf, unsigned int len)
{
  uint8_t device = network_unit(devicespec) + 0x70;

  return int_f5_read(device,'R',len&0xFF,len>>8,(void *)buf,len) == 'C' ? len : FN_ERR_IO_ERROR;
}

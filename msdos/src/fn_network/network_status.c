#include <stdbool.h>
#include <stdint.h>
#include <fujinet-network.h>
#include <fujinet-fuji-msdos.h>
#include <string.h>

uint8_t network_status(const char *devicespec, uint16_t *bw, uint8_t *c, uint8_t *err)
{
  uint8_t device = network_unit(devicespec) + 0x70;
  uint8_t ret = 0;
  struct _sr
  {
    uint16_t bw;
    uint8_t c;
    uint8_t err;
  } sr;

  memset(&sr,0,sizeof(sr));

  ret = int_f5_read(device,'S',0x00,0x00,&sr,sizeof(sr));

  if (bw)
    *bw = sr.bw;
  if (c)
    *c = sr.c;
  if (err)
    *err = sr.err;

  return ret == 'C' ? FN_ERR_OK : FN_ERR_IO_ERROR;
}

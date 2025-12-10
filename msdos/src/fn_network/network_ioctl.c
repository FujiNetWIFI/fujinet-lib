#include <stdbool.h>
#include <stdint.h>
#include <stdarg.h> // because we're using varargs here.
#include <stdlib.h>
#include <fujinet-network.h>
#include <fujinet-fuji-msdos.h>

// Three extra args, dstats, dbuf, dbyt
uint8_t network_ioctl(uint8_t cmd, uint8_t aux1, uint8_t aux2, const char* devicespec, ...)
{
  uint8_t device = network_unit(devicespec) + 0x70;
  va_list ap = NULL;
  uint8_t dstats = 0;
  uint8_t *dbuf = NULL;
  uint16_t dbyt = 0;
  uint8_t ret = 0;

  va_start(ap,devicespec);

  if (ap)
    dstats = va_arg(ap, uint8_t);
  if (ap)
    dbuf = va_arg(ap, uint8_t *);
  if (ap)
    dbyt = va_arg(ap, uint16_t);

  va_end(ap);

  if (dstats == 0x00)
    ret = int_f5(device,cmd,aux1,aux2) == 'C';
  else if (dstats == 0x40) // Payload to computer
    ret = int_f5_read(device,cmd,aux1,aux2,dbuf,dbyt) == 'C';
  else if (dstats == 0x80) // Payload to fujinet
    ret = int_f5_write(device,cmd,aux1,aux2,dbuf,dbyt) == 'C';

  return ret;
}

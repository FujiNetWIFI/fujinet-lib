#include <stdbool.h>
#include <stdint.h>
#include <fujinet-network.h>
#include <fujinet-fuji-msdos.h>

int16_t network_json_query(const char *devicespec, const char *query, char *s)
{
  uint8_t device = network_unit(devicespec) + 0x70;
  uint8_t ret = 0;
  uint8_t c=0, err=0;
  uint16_t bw=0;
  char ch;
  unsigned short offset = 0;

  // Perform query
  ret = int_f5_write(device,'Q',0x00,0x00,(void *)query,256);
  if (ret != 'C')
    return ret;

  // Get # of bytes waiting
  network_status(devicespec,&bw,&c,&err);

  if (!bw)
    return 0;

  if (network_read(devicespec, (uint8_t *)s, bw) > 0)
  {
      // if last char is 0x9b, 0x0A or 0x0D, then set that char to nul, else just null terminate
      ch = s[bw - 1];
      if (ch == 0x9B || ch == 0x0A || ch == 0x0D)
      {
          offset = 1;
      }
      s[bw - offset] = '\0';
  }
  
    // return the string length (minus any trailing EOL char)
	return bw - offset;
}

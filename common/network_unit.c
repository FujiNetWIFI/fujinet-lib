#include <fujinet-network.h>

uint8_t network_unit(const char *devicespec)
{
  if (devicespec[1] == ':')
    return 1;

  if (devicespec[2] == ':')
    return devicespec[1] - '0';

  return 1;
}

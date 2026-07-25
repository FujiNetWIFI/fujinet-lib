#include "network.h"

#include <fujinet-network.h>
#include <fujinet-bus.h>
#include <fujinet-commands.h>
#include <fujinet-err.h>

size_t network_json_strip_newlines(char *buffer, size_t buflen)
{
  uint8_t c;

  // if last char is 0x9b, 0x0A or 0x0D, then set that char to nul, else just null terminate
  while (buflen) {
    c = buffer[buflen - 1];
    if (c != 0x9B && c != 0x0A && c != 0x0D)
      break;
    buflen--;
  }
  buffer[buflen] = '\0';

  return buflen;
}

#include "network_unit_status.h"

#include <fujinet-network.h>
#include <fujinet-bus.h>
#include <fujinet-commands.h>
#include <fujinet-err.h>

#ifdef BUILD_APPLE
#warning "MAX_JSON_QUERY_LEN should be 256 but there are bugs in iwm implementation"
#define MAX_JSON_QUERY_LEN 255
#else /* ! BUILD_APPLE */
#define MAX_JSON_QUERY_LEN 256
#endif /* BUILD_APPLE */

extern NetworkStatus nw_status;

int16_t network_json_query(const char *devicespec, const char *query, char *buffer)
{
  char c;
  int16_t total, read_len;
  FN_ERR err;
  uint16_t avail;
  uint8_t nw_unit = network_unit(devicespec);


  if (!NETCALL_D(FUJICMD_QUERY, nw_unit, query, MAX_JSON_QUERY_LEN))
    return -FN_ERR_IO_ERROR;

  total = 0;
  for (;;) {
    err = network_unit_status(nw_unit, &nw_status);
    if (err)
      return -err;

    avail = nw_status.avail;
    if (!avail)
      break;

    read_len = network_read(devicespec, &buffer[total], avail);
    if (read_len < 0)
      return read_len;

    total += read_len;
  }

  // if last char is 0x9b, 0x0A or 0x0D, then set that char to nul, else just null terminate
  c = buffer[total - 1];
  if (c == 0x9B || c == 0x0A || c == 0x0D)
    total--;
  buffer[total] = '\0';

  return total;
}

FN_ERR network_json_parse(const char *devicespec)
{
  uint8_t err = 0;
  uint8_t nw_unit = network_unit(devicespec);


  // Atari and RS232 use aux2, CoCo and Apple2 use aux1
  if (!NETCALL_A1_A2(FUJICMD_JSON_MODE, nw_unit, 1, 1))
    return FN_ERR_IO_ERROR;

  if (!NETCALL(FUJICMD_PARSE, nw_unit))
    return FN_ERR_IO_ERROR;
  return FN_ERR_OK;
}

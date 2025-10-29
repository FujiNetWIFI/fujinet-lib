#include "fujinet-network.h"
#include "fujinet-bus.h"
#include "fujinet-commands.h"
#include "fujinet-err.h"

#warning "MAX_JSON_QUERY_LEN should be 256 but there are bugs in iwm implementation"
#define MAX_JSON_QUERY_LEN 255

int16_t network_json_query(const char *devicespec, const char *query, char *buffer)
{
  char c;
  int16_t total, read_len;
  uint8_t result;
  uint16_t avail;
  uint8_t status, err;
  uint8_t nw_unit = network_unit(devicespec);


  if (!NETCALL_D(FUJICMD_QUERY, nw_unit, query, MAX_JSON_QUERY_LEN))//strlen(query) + 1))
    return -FN_ERR_IO_ERROR;

  total = 0;
  do {
    result = network_status(devicespec, &avail, &status, &err);
    if (result)
      return -result;

    read_len = fuji_bus_read(FUJI_DEVICEID_NETWORK + nw_unit - 1, &buffer[total], 512);
    if (read_len < 0)
      return -FN_ERR_IO_ERROR;
    total += read_len;
  } while (read_len);

  // if last char is 0x9b, 0x0A or 0x0D, then set that char to nul, else just null terminate
  c = buffer[total - 1];
  if (c == 0x9B || c == 0x0A || c == 0x0D)
    total--;
  buffer[total] = '\0';

  return total;
}

uint8_t network_json_parse(const char *devicespec)
{
  uint8_t err = 0;
  uint8_t nw_unit = network_unit(devicespec);


  if (!NETCALL_A1(FUJICMD_JSON_MODE, nw_unit, 1))
    return FN_ERR_IO_ERROR;

  if (!NETCALL(FUJICMD_PARSE, nw_unit))
    return FN_ERR_IO_ERROR;
  return FN_ERR_OK;
}

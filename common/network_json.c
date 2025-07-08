#include "fujinet-network.h"
#include "fujinet-bus.h"
#include "fujinet-commands.h"
#include <stdint.h>
#include <string.h>

#define MAX_JSON_QUERY_LEN 256

int16_t network_json_query(const char *devicespec, const char *query, char *buffer)
{
  char c;
  int16_t total, read_len;
  uint16_t query_len;
  uint8_t nw_unit = network_unit(devicespec);


  query_len = strlen(query) + 1; // add 1 for nul
  if (!fuji_bus_call(FUJI_DEVICEID_NETWORK, nw_unit, FUJICMD_QUERY,
		     FUJI_FIELD_AUX1 | FUJI_FIELD_AUX2,
		     query_len & 0xff, query_len >> 8, 0, 0,
		     query, MAX_JSON_QUERY_LEN, NULL, 0))
    return FN_ERR_IO_ERROR;

  total = 0;
  do {
    read_len = fuji_bus_read(FUJI_DEVICEID_NETWORK, nw_unit, &buffer[total], 512);
    if (read_len < 0)
      return FN_ERR_IO_ERROR;
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


  if (!fuji_bus_call(FUJI_DEVICEID_NETWORK, nw_unit, FUJICMD_JSON_MODE,
		     FUJI_FIELD_NONE, 0, 0, 0, 0, NULL, 0, NULL, 0))
    return FN_ERR_IO_ERROR;

  return fuji_bus_call(FUJI_DEVICEID_NETWORK, nw_unit, FUJICMD_PARSE,
		       FUJI_FIELD_NONE, 0, 0, 0, 0, NULL, 0, NULL, 0);
}

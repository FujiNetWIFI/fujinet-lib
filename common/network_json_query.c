#include <fujinet-network.h>

int16_t network_json_query_common(const char *devicespec, const char *query, char *buffer)
{
  int16_t total, read_len;
  FN_ERR err;
  uint16_t avail;
  uint8_t nw_unit = network_unit(devicespec);


  if (!NETCALL_D(NETCMD_QUERY, nw_unit, query, MAX_JSON_QUERY_LEN))
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

  return network_json_strip_newlines(buffer, total);
}

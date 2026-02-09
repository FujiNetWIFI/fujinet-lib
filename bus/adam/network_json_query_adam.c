#include <fujinet-network.h>
#include <string.h>

#define MAX_ADAM_PACKET_REPLY 1024

int16_t network_json_query_adam(const char *devicespec, const char *query, char *buffer)
{
  int16_t read_len;
  uint8_t nw_unit = network_unit(devicespec);


  if (!NETCALL_D(FUJICMD_QUERY, nw_unit, query, strlen(query)))
    return -FN_ERR_IO_ERROR;

  read_len = network_read_nb_adam(devicespec, buffer, MAX_ADAM_PACKET_REPLY);
  return network_json_strip_newlines(buffer, read_len);
}

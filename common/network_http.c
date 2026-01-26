#include "fujinet-network.h"
#include "fujinet-commands.h"

#ifndef _CMOC_VERSION_
#include <string.h>
#endif /* _CMOC_VERSION_ */

FN_ERR network_http_post(const char *devicespec, const char *data)
{
  FN_ERR err;


  err = network_http_set_channel_mode(devicespec, HTTP_CHAN_MODE_POST_SET_DATA);
  if (err)
    return err;
  err = network_write(devicespec, (uint8_t *) data, strlen(data));
  if (err)
    return err;
  return network_http_set_channel_mode(devicespec, HTTP_CHAN_MODE_BODY);
}

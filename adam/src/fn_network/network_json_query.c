/**
 * @brief   JSON query.
 * @author  Geoff Oltmans
 * @email   oltmansg at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose ---
 * @param devicespec The Device Specification "N:..."
 * @param query The query path string
 * @param s returned query data
 * @return AdamNet unit number.
 */

#include <stdint.h>
#include "fujinet-network.h"
#include <stdio.h>
#include "response.h"
#include <eos.h>

int16_t network_json_query(const char *devicespec, const char *query, char *s)
{
  char q[256];
  uint8_t err = 0;
  uint8_t u = network_unit_adamnet(devicespec);
  DCB * dcb = 0;

  if (!u) {
    return -FN_ERR_NO_DEVICE;
  }

  memset(response,0,sizeof(response));
  memset(q,0,sizeof(q));
  q[0] = 'Q';
  strncat(q,query,sizeof(q));

  while (1)
  {
    err = eos_write_character_device(u,q,sizeof(q));

    if (err == ADAMNET_TIMEOUT)
      continue;
    else if (err == ADAMNET_OK)
      break;  
    else
      return -FN_ERR_IO_ERROR;
  }

  //get the DCB...
  dcb = eos_find_dcb(u); // Replace with net device

  while (1)
  {
    err = eos_read_character_device(u,response,RESPONSE_SIZE);

    if (err == ADAMNET_TIMEOUT)
      continue;
    else if (err == ADAMNET_OK)
      break;  
    else
      return -FN_ERR_IO_ERROR;
  }

  memcpy(s,response, dcb->len);
  return dcb->len;
}

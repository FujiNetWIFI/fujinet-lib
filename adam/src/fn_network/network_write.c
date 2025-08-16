/**
 * @brief   Write to network via Adamnet.
 * @author  Geoff Oltmans
 * @email   oltmansg at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose ---
 * @param devicespec The Device Specification "N:..."
 * @param buf – Buffer
 * @param len – length
 * @return fujinet-network error code (See FN_ERR_* values)
 */

#include <stdint.h>
#include <eos.h>
#include "fujinet-network.h"
#include "fujinet-network-adam.h"
#include "response.h"

uint8_t network_write(const char *devicespec, const uint8_t *buf, uint16_t len)
{
  uint16_t send_len;
  uint8_t err = 0;


  response[0] = 'W';
  uint8_t u = network_unit_adamnet(devicespec);

  if (!u)
    return FN_ERR_NO_DEVICE;

  while (len)
  {
    send_len = (len > RESPONSE_SIZE-1) ? RESPONSE_SIZE-1 : len;
    memcpy(&response[1],buf,send_len);
    len -= send_len;
    buf += send_len;

    while (1)
    {
      err = eos_write_character_device(u,response,send_len+1);

      if (err == ADAMNET_TIMEOUT)
          continue;
      else if (err == ADAMNET_OK)
          break;
      else
          return FN_ERR_IO_ERROR;
    }
  }
  return FN_ERR_OK;
}

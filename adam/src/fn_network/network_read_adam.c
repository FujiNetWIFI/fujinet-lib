/**
 * @brief   read from adamnet.
 * @author  Geoff Oltmans
 * @email   oltmansg at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose ---
 * @param devicespec The Device Specification "N:..."
 * @param buf buffer to read into
 * @param len length of buffer
 * @return fujinet error code.
 */

#include <stdint.h>
#include "response.h"
#include <eos.h>
#include "fujinet-network.h"
#include <string.h>
#include "fujinet-network-adam.h"

int16_t network_read_adam(char* devicespec, uint8_t *buf, uint16_t len)
{
  uint8_t u = network_unit_adamnet(devicespec);
  uint8_t err = 0;

  if (!u) {
    return FN_ERR_BAD_CMD;
  }

  DCB *dcb = eos_find_dcb(u); // Replace with net device

  while (1)
  {
    err = eos_read_character_device(u, response, RESPONSE_SIZE);

    if (err == ADAMNET_TIMEOUT)
      continue;
    else if (err == ADAMNET_OK)
    {
      memcpy(buf,response,dcb->len);
      return FN_ERR_OK;
    }
    else
      return FN_ERR_IO_ERROR;
  }
}

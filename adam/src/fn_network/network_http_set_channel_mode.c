/**
 * @brief   Set channel mode.
 * @author  Geoff Oltmans
 * @email   oltmansg at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose ---
 * @param devicespec The Device Specification "N:..."
 * @param mode mode
 * @return AdamNet unit number.
 */

#include <stdint.h>
#include "fujinet-network.h"
#include <eos.h>

uint8_t network_http_set_channel_mode(const char *devicespec, uint8_t mode)
{
  uint8_t err;
  uint8_t SCM[2];
  SCM[0] = 0xFC; // Set channel mode
  SCM[1] = mode; // to JSON mode

  while (1)
  {
    err = eos_write_character_device(u,SCM,sizeof(SCM));

    if (err == ADAMNET_TIMEOUT)
      continue;
    else if (err == ADAMNET_OK)
      return FN_ERR_OK;
    else
      return FN_ERR_IO_ERROR;
  }
}

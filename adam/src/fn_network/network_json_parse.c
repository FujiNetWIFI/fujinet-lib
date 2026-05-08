/**
 * @brief   Parse JSON after a network open.
 * @author  Geoff Oltmans
 * @email   oltmansg at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose ---
 * @param devicespec The Device Specification "N:..."
 * @return AdamNet unit number.
 */

#include <stdint.h>
#include "fujinet-network.h"
#include <eos.h>

uint8_t network_json_parse(const char *devicespec)
{
  uint8_t err = 0;
  uint8_t u = network_unit_adamnet(devicespec);

  if (!u)
    return FN_ERR_NO_DEVICE;

  while (1)
  {
    err =  eos_write_character_device(u,"P",1);
    
    if (err == ADAMNET_TIMEOUT)
      continue;
    else if (err == ADAMNET_OK)
      return FN_ERR_OK;
    else
      return FN_ERR_IO_ERROR;
  }
}

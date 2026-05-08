/**
 * @brief   Open a network connection.
 * @author  Geoff Oltmans
 * @email   oltmansg at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose ---
 * @param devicespec The Device Specification "N:..."
 * @param devicespec The Device Specification "N:..."
 * @param trans translation mode
 * @return AdamNet unit number.
 */

#include <stdint.h>
#include <eos.h>
#include "fujinet-network.h"
#include "fujinet-network-adam.h"

/**
 * @brief Open network connection
 * @param devicespec Device spec to use "N:..." with URL
 * @param mode connection mode
 * @param trans Translation mode
 * @return FN_error code.
 */
uint8_t network_open(const char* devicespec, uint8_t mode, uint8_t trans)
{
  struct _o
  {
    unsigned char cmd;
    unsigned char mode;
    unsigned char trans;
    char url[256];
  } o;

  uint8_t err = 0;

  uint8_t u = network_unit_adamnet(devicespec);

  if (!u)
    return FN_ERR_BAD_CMD;
  
  o.cmd = 'O'; // open
  o.mode = mode;
  o.trans = trans;

  strncpy(o.url, devicespec, sizeof(o.url));

  while (1)
  {
    err = eos_write_character_device(u, (unsigned char *)o, sizeof(o));

    if (err == ADAMNET_TIMEOUT)
      continue;
    else if (err == ADAMNET_OK)
      return FN_ERR_OK;
    else
      return FN_ERR_IO_ERROR;
  }
}

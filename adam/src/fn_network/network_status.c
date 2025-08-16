/**
 * @brief   Return network status for adamnet.
 * @author  Geoff Oltmans
 * @email   oltmansg at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose ---
 * @param devicespec The Device Specification "N:..."
 * @param bw pointer to count of bytes waiting
 * @param c pointer to flag indicated whether we're connected
 * @param err pointer to variable to return extended error status code
 * @return fujinet error status.
 */

#include <stdint.h>
#include <eos.h>
#include "fujinet-network.h"
#include "fujinet-network-adam.h"
#include "response.h"

uint8_t network_status(const char *devicespec, uint16_t *bw, uint8_t *c, uint8_t *err)
{
    struct _ns
    {
        unsigned short bw;
        bool connected;
        unsigned char err;
    } ns;

    uint8_t e = 0;
    uint8_t u = network_unit_adamnet(devicespec);

    if (!u)
        return FN_ERR_NO_DEVICE;

    while (1)
    {
      e = eos_write_character_device(u,"S",1);

      if (e == ADAMNET_TIMEOUT)
        continue;
      else if (e == ADAMNET_OK)
        break;  
      else
        return FN_ERR_IO_ERROR;
    }

    while (1)
    {
      e = eos_read_character_device(u,response,RESPONSE_SIZE);

      if (e == ADAMNET_TIMEOUT)
        continue;
      else if (e == ADAMNET_OK)
        break;  
      else
        return FN_ERR_IO_ERROR;
    }

    memcpy(&ns,response,sizeof(ns));

    if (bw)
        *bw = ns.bw;

    if (c)
        *c = ns.connected;

    if (err)
        *err = ns.err;

    return FN_ERR_OK;
}

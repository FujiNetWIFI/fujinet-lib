/**
 * @brief   Return proper unit # for adamnet.
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose ---
 * @param devicespec The Device Specification "N:..."
 * @return AdamNet unit number.
 */

#include "fujinet-network-adam.h"

uint8_t network_unit_adamnet(char *devicespec)
{
    uint8_t u=0;
    
    if (!(u = network_unit(devicespec)))
        return 0;
    else
    {
        u += (NETWORK_DEVICE_ID_START - 1);
        return u;
    }
}

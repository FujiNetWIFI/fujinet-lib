/**
 * @brief   Network DCBs
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose AdamNet DCBs for Network devices
 */

#include <eos.h>
#include "fujinet-network-adam.h"

/**
 * @brief pointers to network DCBs, set by network_init()
 */
DCB *network_dcb[MAX_NETWORK_DEVICES];

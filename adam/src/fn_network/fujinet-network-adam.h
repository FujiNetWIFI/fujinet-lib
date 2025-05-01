/**
 * @brief   Adam-specific Defines.
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose ---
 */

#ifndef FUJINET_NETWORK_ADAM_H
#define FUJINET_NETWORK_ADAM_H

#include <stdint.h>
#include <eos.h>

/**
 * @brief total # of network devices defined by firmware
 */
#define MAX_NETWORK_DEVICES 2

/**
 * @brief Starting AdamNet device ID for network devices.
 */
#define NETWORK_DEVICE_ID_START 0x09

/**
 * @brief defined in network_dcbs.c
 */
extern DCB *network_dcb[MAX_NETWORK_DEVICES];

/**
 * @brief Return proper unit # for adamnet.
 * @param devicespec The Device Specification "N:..."
 * @return AdamNet unit number.
 */
uint8_t network_unit_adamnet(char *devicespec);

#endif /* FUJINET_NETWORK_ADAM_H */

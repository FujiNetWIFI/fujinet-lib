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
#include "fujinet-fuji.h"

/**
 * @brief AdamNet Device ID for FujiNet Device
 */
#define FUJINET_DEVICE_ID 0x0F

#define ADAMNET_SEND_APPKEY_READ  0xDD
#define ADAMNET_SEND_APPKEY_WRITE 0xDE

#define MAX_APPKEY_LEN 64

extern uint16_t fn_adam_creator_id;
extern uint8_t fn_adam_app_id;

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
uint8_t network_unit_adamnet(const char *devicespec);

/**
 * @brief Return proper unit # for adamnet.
 * @param devicespec The Device Specification "N:..."
 * @param buf Buffer to read data into
 * @param len Length of the buffer
 * @return Fujinet error code.
 */
int16_t network_read_adam(const char* devicespec, uint8_t *buf, uint16_t len);

#endif /* FUJINET_NETWORK_ADAM_H */

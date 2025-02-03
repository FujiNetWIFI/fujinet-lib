/**
 * @brief   FujiNet Device header
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose Private
 */

#ifndef FUJINET_FUJI_COCO_H
#define FUJINET_FUJI_COCO_H

#include "stdbool-coco.h"

/**
 * @brief Error code for success
 */
#define BUS_SUCCESS 1

/**
 * @brief error code for error
 */
#define BUS_ERROR 144

/**
 * @brief Is FujiNet ready?
 */
#define FUJICMD_READY 0x00
void bus_ready(void);

/**
 * @brief Get most recent response from Fuji device (OP_FUJI)
 * @param buf Target buffer
 * @param len length of bytes to get
 * @return Success status, true if the response was received successfully.
 */
#define FUJICMD_GET_RESPONSE 0x01
bool fuji_get_response(uint8_t *buf, int len);

/**
 * @brief Get the error code from last Fuji command
 * @return Error condition, true if error was received (or if failed to receive an error).
 */
#define FUJICMD_SEND_ERROR 0x02
bool fuji_get_error(void);

#endif /* FUJINET_FUJI_COCO_H */

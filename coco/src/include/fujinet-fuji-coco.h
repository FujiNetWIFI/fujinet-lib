/**
 * @brief   FujiNet Device header
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose Private
 */

#ifndef FUJINET_FUJI_COCO_H
#define FUJINET_FUJI_COCO_H

/**
 * @brief Is FujiNet ready?
 */
#define FUJICMD_READY 0x00
void bus_ready(void);

/**
 * @brief Get most recent response
 * @param opcode OP_FUJI or OP_NET
 * @param buf Target buffer
 * @param len length of bytes to get
 */
#define FUJICMD_GET_RESPONSE 0x01
uint8_t io_get_response(uint8_t opcode, uint8_t *buf, int len);

#endif /* FUJINET_FUJI_COCO_H */

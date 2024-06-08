/**
 * @brief   Return error code of previous sub device operation
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose opcode is OP_FUJI or OP_NET
 */

#include <cmoc.h>
#include <coco.h>
#include <dw.h>
#include <fujinet-fuji-coco.h>

uint8_t bus_error(uint8_t opcode)
{
    struct _senderrorcmd
    {
        uint8_t opcode;
        uint8_t cmd;
    } sec;

    uint8_t err = 0;
    
    sec.opcode = opcode;
    sec.cmd = FUJICMD_SEND_ERROR;

    bus_ready();
    dwwrite((uint8_t *)&sec, sizeof(sec));
    dwread((uint8_t *)&err, sizeof(err));

    return err;
}

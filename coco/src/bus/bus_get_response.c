/**
 * @brief   Get Bus Response for OP_FUJI or OP_NET
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose opcode is OP_FUJI or OP_NET
 * @param   opcode OP_FUJI or OP_NET
 * @param   buf Target buffer (needs to be at least len)
 */

#include <cmoc.h>
#include <coco.h>
#include <dw.h>
#include <fujinet-fuji-coco.h>

uint8_t bus_get_response(uint8_t opcode, uint8_t *buf, int len)
{
    struct _getresponsecmd
    {
        uint8_t opcode;
        uint8_t command;
    } grc;

    uint8_t z=0;
    
    grc.opcode = opcode;
    grc.command = FUJICMD_GET_RESPONSE;

    bus_ready();
    dwwrite((uint8_t *)&grc, sizeof(grc));
    dwread((uint8_t *)buf, len);
    
    return z;
}

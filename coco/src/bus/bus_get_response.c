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
        uint8_t p1;
        uint8_t p2;
        uint16_t auxs;
    } grc;
    
    uint8_t z=sizeof(grc);

    grc.opcode = opcode;

    if (opcode == OP_NET) {
        grc.p1 = 1; // Firmware appears to ignore unit
        grc.p2 = FUJICMD_GET_RESPONSE;
        grc.auxs = len;
    } else { // OP_FUJI
        grc.p1 = FUJICMD_GET_RESPONSE;
        z=2;   
    }

    bus_ready();
    dwwrite((uint8_t *)&grc, z);

    // Return 0 on a successful read (dwread returns 1)
    return !dwread((uint8_t *)buf, len);
}
/*
 struct _getresponsecmd
    {
        byte opcode;
        byte id;
        byte command;
        int len;
    } grc;
    
    grc.opcode = OP_NET;
    grc.id = 1;
    grc.command = FUJICMD_GET_RESPONSE;
    grc.len = len;

    bus_ready();
    dwwrite((byte *)&grc, sizeof(grc));

    // Returns 0 on a successful read
    return !dwread((byte *)buf, len);*/
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
    struct _errcmd
    {
        uint8_t opcode;
        uint8_t p1;
        uint8_t p2;
        uint16_t auxs;
    } ec;

    
    uint8_t z=sizeof(ec);

    ec.opcode = opcode;

    if (opcode == OP_NET) {
        ec.p1 = 1; // Firmware appears to ignore unit
        ec.p2 = FUJICMD_SEND_ERROR;
        ec.auxs = 0;
    
    } else { // OP_FUJI
        ec.p1 = FUJICMD_SEND_ERROR;
        z=2;   
    }

    bus_ready();
    dwwrite((uint8_t *)&ec, z);
    
    // Read Error/Status code
    if (dwread(&z,1)) {
        // Receiving a 1 means success. Change 1 to 0 since caller expects 0 for success
        if (z == 1) {
            z = 0;
        }
        return z;
    }

    return 1; // IO ERROR;
}

/**
 * @brief   Bus ready?
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose Block until device ready.
 */

#include <dw.h>
#include <fujinet-fuji-pmd85.h>

void bus_ready(void)
{
    struct _readycmd
    {
        uint8_t opcode;
        uint8_t command;
    } rc;

    uint8_t z = 0, r = 0;
    
    rc.opcode = OP_FUJI;
    rc.command = FUJICMD_READY;
    
    while (!z)
    {
        dwwrite((uint8_t *)&rc, sizeof(rc));
        z = dwread(&r, sizeof(r));
    }  
}

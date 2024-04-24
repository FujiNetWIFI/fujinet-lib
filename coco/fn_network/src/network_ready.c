/**
 * @brief   Wait for Network Ready
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose drivewire functions required.
 */

#include <dw.h>

extern unsigned char network_get_unit_from_devicespec(const char devicespec);

#define OP_NET 0xE3
#define CMD_READY 0x00

void network_ready(const char devicespec)
{
    struct _readycmd
    {
        byte opcode;
        byte id;
        byte command;
        byte aux1;
        byte aux2;
    } rc;

    unsigned char u = network_get_unit_from_devicespec(devicespec);
    byte z=0, r=0;
    
    rc.opcode = OP_NET;
    rc.id = u;
    rc.command = CMD_READY;
    rc.aux1 = rc.aux2 = 0;
    
    while (!z) // non-zero flag indicates not ready.
    {
        dwwrite((byte *)&rc,sizeof(rc));
        z = dwread((byte *)&r,sizeof(r));
    }
    
}

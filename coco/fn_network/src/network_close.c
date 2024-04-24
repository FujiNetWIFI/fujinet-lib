/**
 * @brief   Close network connection
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose Unit number derived from N: filespec.
 */

#include <dw.h>

#define OP_NET 0xE3
#define CMD_CLOSE 'C'

extern unsigned char network_get_unit_from_devicespec(const char *devicespec);
extern unsigned char network_ready(const char *devicespec);

unsigned char network_close(const char *devicespec)
{
    unsigned char u = network_get_unit_from_devicespec(devicespec);

    struct _closecmd
    {
        unsigned char opcode;
        unsigned char id;
        unsigned char command;
        unsigned char aux1;
        unsigned char aux2;
    } cc;

    cc.opcode = OP_NET;
    cc.id = u;
    cc.command = CMD_CLOSE;
    cc.aux1 = cc.aux2 = 0;

    // Wait for ready
    network_ready(devicespec);

    // Send command
    dwwrite((unsigned char *)&cc, sizeof(cc));

    return 1; // always success.
}

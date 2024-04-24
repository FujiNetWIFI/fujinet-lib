/**
 * @brief   Return most recent Network Error
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose ---
 */

#include <dw.h>

extern unsigned char network_ready(const char *devicespec);
extern unsigned char network_get_unit_from_devicespec(const char *devicespec);

#define OP_NET 0xE3
#define CMD_ERROR 0x02

unsigned char network_error(const char *devicespec)
{
    struct _errcmd
    {
        unsigned char opcode;
        unsigned char id;
        unsigned char command;
        unsigned char aux1;
        unsigned char aux2;
    } ec;

    unsigned char z=0;
    unsigned char err=0;
    
    ec.opcode = OP_NET;
    ec.id = network_get_unit_from_devicespec(devicespec);
    ec.command = CMD_ERROR;
    ec.aux1 = ec.aux2 = 0;

    network_ready(devicespec);
    
    dwwrite((unsigned char *)&ec, sizeof(ec));
    z = dwread(&err,1);
   
    return z ? z : err;
}

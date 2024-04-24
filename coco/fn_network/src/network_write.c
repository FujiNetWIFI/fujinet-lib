/**
 * @brief   Write to Network device specified by N: devicespec
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose ---
 */

#include <dw.h>

#define OP_NET 0xE3
#define CMD_WRITE 'W'

extern void network_ready(const char *devicespec);
extern unsigned char network_error(const char *devicespec);
extern unsigned char network_get_unit_from_devicespec(const char *devicespec);

unsigned char network_write(char* devicespec, unsigned char *buf, unsigned short len)
{
    unsigned char z = 0;

    struct _writecmd
    {
        unsigned char opcode;
        unsigned char id;
        unsigned char command;
        unsigned int len;
        unsigned char *b;
    } wc;

    wc.opcode = OP_NET;
    wc.id = network_get_unit_from_devicespec(devicespec);
    wc.command = CMD_WRITE;
    wc.len = len;
    wc.b = buf;
    
    network_ready(devicespec);
    dwwrite((unsigned char *)&wc, sizeof(wc)+len);
    return 1;

}

/**
 * @brief   Network read from N: device
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose ---
 */

#include <dw.h>

#define OP_NET 0xE3
#define CMD_READ 'R'

extern void network_ready(const char *devicespec);
extern unsigned char network_error(const char *devicespec);
extern unsigned char network_get_response(const char *devicespec, unsigned char *buf, unsigned short len);
extern unsigned char network_get_unit_from_devicespec(const char *devicespec);

int network_read(char* devicespec, unsigned char *buf, unsigned short len)
{
    unsigned char z = 0;

    struct _readcmd
    {
        unsigned char opcode;
        unsigned char id;
        unsigned char command;
        unsigned int len;
    } rc;

    rc.opcode = OP_NET;
    rc.id = network_get_unit_from_devicespec(devicespec);
    rc.command = CMD_READ;
    rc.len = len;

    network_ready(devicespec);
    dwwrite((unsigned char *)&rc, sizeof(rc));
    network_get_response(devicespec, buf, len);

    return network_error(devicespec);
}

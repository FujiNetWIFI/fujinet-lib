/**
 * @brief   Return response buffer from ESP32
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @param   devicespec (N: devicespec)
 * @param   buf The target for response
 * @param   Length of buffer
 * @return  error code from network_error()
 * @verbose ---
 */

#include <dw.h>

#define OP_NET 0xE3
#define CMD_RESPONSE 0x01

extern unsigned char network_get_unit_from_devicespec(const char *devicespec);
extern void network_ready(const char *devicespec);

unsigned char network_get_response(const char *devicespec, unsigned char *buf, int len)
{
    struct _getresponsecmd
    {
        unsigned char opcode;
        unsigned char id;
        unsigned char command;
        int len;
    } grc;

    unsigned char z=0;
    
    grc.opcode = OP_NET;
    grc.id = network_get_unit_from_devicespec(devicespec);
    grc.command = CMD_RESPONSE;
    grc.len = len;

    network_ready(devicespec);
    dwwrite((unsigned char *)&grc, sizeof(grc));
    dwread((unsigned char *)buf, len);
    
    return z;
}

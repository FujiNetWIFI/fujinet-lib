/**
 * @brief   Get status of network connection
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose specified by N: devicespec
 */

#include <dw.h>

#define OP_NET 0xE3
#define CMD_STATUS 'S'

extern void network_ready(const char *devicespec);
extern unsigned char network_get_unit_from_devicespec(const char *devicespec);
extern unsigned char network_get_response(const char *devicespec, unsigned char *buf, unsigned short len);
extern unsigned char network_error(const char *devicespec);

unsigned char network_status(char *devicespec, unsigned short *bw, unsigned char *c, unsigned char *err)
{
    unsigned char z = 0;
    
    struct _statuscmd
    {
        unsigned char opcode;
        unsigned char id;
        unsigned char command;
        unsigned char aux1;
        unsigned char aux2;
    } sc;

    struct _networkStatus
    {
        unsigned short bw;
        unsigned char c;
        unsigned char err;
    } ns;

    sc.opcode = OP_NET;
    sc.id = network_get_unit_from_devicespec(devicespec);
    sc.command = CMD_STATUS;
    sc.aux1 = sc.aux2 = 0;

    network_ready(devicespec);
    dwwrite((unsigned char *)&sc, sizeof(sc));
    network_get_response(devicespec,(unsigned char *)&ns, sizeof(ns));

    if (bw)
        *bw = ns.bw;

    if (c)
        *c = ns.c;

    if (err)
        *err = ns.err;
    
    return network_error(devicespec);    
}

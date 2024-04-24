/**
 * @brief   Perform JSON query
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose ---
 */

#include <dw.h>

#define OP_NET 0xE3
#define CMD_QUERY_JSON 'Q'

extern void network_ready(const char *devicespec);
extern unsigned char network_error(const char *devicespec);
extern unsigned short network_read(const char *devicespec, unsigned char *buf, unsigned short len);
extern unsigned char network_get_unit_from_devicespec(const char *devicespec);
extern unsigned char network_status(const char *devicespec, unsigned short *bw, unsigned char *c, unsigned char *err);

int network_json_query(char *devicespec, char *query, char *s)
{
    struct _queryjsoncmd
    {
        byte opcode;
        byte id;
        byte command;
        byte aux1;
        byte aux2;
        char query[256];
    } qj;

    unsigned short bw=0;
    unsigned char c=0;
    unsigned char err=0;
    
    qj.opcode = OP_NET;
    qj.id = network_get_unit_from_devicespec(devicespec);
    qj.command = CMD_QUERY_JSON;
    qj.aux1 = qj.aux2 = 0;
    memset(qj.query,0,256);
    strcpy(qj.query,query);

    network_ready(devicespec);
    dwwrite((byte *)&qj, sizeof(qj));

    network_ready(devicespec);
    network_status(devicespec,&bw,&c,&err);

    if (!bw)
    {
        return network_error(devicespec);
    }

    if (err != 1)
    {
        return network_error(devicespec);
    }

    return network_read(devicespec,(unsigned char *)s,bw);
}

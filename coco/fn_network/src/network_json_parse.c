/**
 * @brief   Parse JSON on open devicespec
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose ---
 */

#include <dw.h>

#define OP_NET 0xE3
#define CMD_PARSE_JSON 'P'
#define CMD_SET_CHANNEL_MODE 0xFC
#define CHANNEL_MODE_JSON 1

extern void network_ready(const char *devicespec);
extern unsigned char network_get_unit_from_devicespec(const char *devicespec);
extern unsigned char network_error(const char *devicespec);

unsigned char network_json_parse(const char *devicespec)
{
    struct _setchannelmodecmd
    {
        unsigned char opcode;
        unsigned char id;
        unsigned char command;
        unsigned char mode;
        unsigned char aux2;
    } scm;

    struct _parsejsoncmd
    {
        unsigned char opcode;
        unsigned char id;
        unsigned char command;
        unsigned char aux1;
        unsigned char aux2;
    } pj;

    scm.opcode = OP_NET;
    scm.id = network_get_unit_from_devicespec(devicespec);
    scm.command = CMD_SET_CHANNEL_MODE;
    scm.mode = CHANNEL_MODE_JSON;
    scm.aux2 = 0;

    network_ready(devicespec);
    dwwrite((unsigned char *)&scm, sizeof(scm));
    
    pj.opcode = OP_NET;
    pj.id = network_get_unit_from_devicespec(devicespec);
    pj.command = CMD_PARSE_JSON;
    pj.aux1 = pj.aux2 = 0;

    network_ready(devicespec);
    
    dwwrite((unsigned char *)&pj, sizeof(pj));
    
    return network_error(devicespec);
}

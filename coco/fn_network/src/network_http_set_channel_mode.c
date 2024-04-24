/**
 * @brief   Set HTTP channel mode
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose ---
 */

#include <dw.h>

#define OP_NET 0xE3
#define CMD_SET_HTTP_CHANNEL_MODE 'M'

extern unsigned char network_get_unit_from_devicespec(const char *devicespec);
extern void network_ready(const char *devicespec);
extern unsigned char network_error(const char *devicespec);

unsigned char network_http_set_channel_mode(char *devicespec, unsigned char mode)
{
    struct _sethttpchannelmodecmd
    {
        unsigned char opcode;
        unsigned char id;
        unsigned char command;
        unsigned char mode;
        unsigned char aux2;
    } hscm;

    hscm.opcode = OP_NET;
    hscm.id = network_get_unit_from_devicespec(devicespec);
    hscm.command = CMD_SET_HTTP_CHANNEL_MODE;
    hscm.mode = mode;
    hscm.aux2 = 0;

    network_ready(devicespec);
    dwwrite((unsigned char *)&hscm, sizeof(hscm));

    return network_error(devicespec);
}

/**
 * @brief   Open Network Connection specified by N: devicespec
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose ---
 */

#include <dw.h>
#include <cmoc.h>

#define OP_NET 0xE3
#define CMD_OPEN 'O'

extern unsigned char network_get_unit_from_devicespec(const char *devicespec);
extern void network_ready(const char *devicespec);
extern unsigned char network_error(const char *devicespec);

unsigned char net_open(const char *devicespec, unsigned char mode, unsigned char trans)
{
    struct _opencmd
    {
        unsigned char opcode;
        unsigned char id;
        unsigned char command;
        unsigned char mode;
        unsigned char trans;
        char devicespec[256];
    } oc;

    oc.opcode = OP_NET;
    oc.id = network_get_unit_from_devicespec(devicespec);
    oc.command = CMD_OPEN;
    oc.mode = mode;
    oc.trans = trans;

    memset(oc.devicespec,0,256);
    strcpy(oc.devicespec,devicespec);

    // wait for ready
    network_ready(devicespec);
    
    // Send open command
    dwwrite((unsigned char *)&oc, sizeof(oc));

    // check status of open
    network_ready(devicespec);
    
    // Return result of open
    return network_error(devicespec);
}

#include <cmoc.h>
#include <coco.h>
#include <dw.h>
#include <fujinet-fuji-coco.h>
#include <fujinet-network-coco.h>
#include <fujinet-network.h>

int network_read_coco(char* devicespec, byte *buf, unsigned int len)
{
    byte z = 0;

    struct _readcmd
    {
        byte opcode;
        byte id;
        byte command;
        unsigned int len;
    } rc;

    rc.opcode = OP_NET;
    rc.id = network_unit(devicespec);
    rc.command = 'R';
    rc.len = len;

    bus_ready();
    dwwrite((byte *)&rc, sizeof(rc));
    net_get_response(network_unit(devicespec), buf, len);

    return len; /* Yeah, I know, FIXME. */
}

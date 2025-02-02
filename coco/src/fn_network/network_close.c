#include <cmoc.h>
#include <coco.h>
#include <dw.h>
#include <fujinet-network.h>
#include <fujinet-fuji-coco.h>
#include <fujinet-network-coco.h>

uint8_t network_close(const char* devicespec)
{
    struct _nc
    {
        uint8_t opcode;
        uint8_t unit;
        uint8_t cmd;
        uint16_t auxs;
    } nc;

    nc.opcode = OP_NET;
    nc.unit = network_unit(devicespec);
    nc.cmd = NETCMD_CLOSE;
    nc.auxs = 0;

    bus_ready();

    dwwrite((uint8_t *)&nc, sizeof(nc));
    
    return bus_error(OP_NET);
}

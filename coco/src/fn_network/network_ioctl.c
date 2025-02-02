#include <cmoc.h>
#include <coco.h>
#include <dw.h>
#include <fujinet-fuji-coco.h>
#include <fujinet-network.h>
#include <fujinet-network-coco.h>

uint8_t network_ioctl(uint8_t cmd, uint8_t aux1, uint8_t aux2, const char* devicespec, ...)
{
    struct _ioctl
    {
        uint8_t opcode;
        uint8_t unit;
        uint8_t cmd;
        uint8_t aux1;
        uint8_t aux2;
        char devicespec[256];
    } ioctl;

    ioctl.opcode = OP_NET;
    ioctl.unit = network_unit(devicespec);
    ioctl.cmd = cmd;
    ioctl.aux1 = aux1;
    ioctl.aux2 = aux2;
    strcpy(ioctl.devicespec,devicespec);

    bus_ready();
    dwwrite((uint8_t *)&ioctl, sizeof(ioctl));
    
    return bus_error(OP_NET) == BUS_SUCCESS;
}

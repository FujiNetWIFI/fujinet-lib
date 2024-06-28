#include <cmoc.h>
#include <coco.h>
#include <dw.h>
#include <fujinet-fuji-coco.h>
#include <fujinet-network.h>
#include <fujinet-network-coco.h>

uint8_t network_json_parse(char *devicespec)
{
    struct _jp
    {
        uint8_t opcode;
        uint8_t unit;
        uint8_t cmd;
        uint8_t aux1;
        uint8_t aux2;
    } jp;

    jp.opcode = OP_NET;
    jp.unit = network_unit(devicespec);
    jp.cmd = 'P';
    jp.aux1 = jp.aux2 = 0;

    bus_ready();
    dwwrite((uint8_t *)&jp, sizeof(jp));
    bus_ready(); // we want to be sure parse completes.

    return bus_error(OP_NET) == BUS_SUCCESS;
}

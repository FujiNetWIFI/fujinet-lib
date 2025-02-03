#include <cmoc.h>
#include <coco.h>
#include <dw.h>
#include <fujinet-fuji-coco.h>
#include <fujinet-network-coco.h>
#include <fujinet-network.h>

uint8_t network_write(const char* devicespec, const uint8_t *buf, uint16_t len)
{
    struct _w
    {
        uint8_t opcode;
        uint8_t unit;
        uint8_t cmd;
        uint16_t len;
    } w;

    w.opcode = OP_NET;
    w.unit = network_unit(devicespec);
    w.cmd = 'W';
    w.len = len;

    bus_ready();
    dwwrite((uint8_t *)&w,sizeof(w));
    dwwrite((uint8_t *)buf, len);
    
    return network_get_error(w.unit);
}

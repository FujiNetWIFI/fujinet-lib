#include <cmoc.h>
#include <coco.h>
#include <dw.h>
#include <fujinet-fuji-coco.h>
#include <fujinet-network-coco.h>
#include <fujinet-network.h>

uint8_t network_status(const char *devicespec, uint16_t *bw, uint8_t *c, uint8_t *err)
{
    struct _s
    {
        uint8_t opcode;
        uint8_t unit;
        uint8_t cmd;
        uint8_t aux1;
        uint8_t aux2;
    } s;

    struct _sr
    {
        uint16_t bw;
        uint8_t c;
        uint8_t err;
    } sr;
    
    uint8_t status;
    uint8_t unit = network_unit(devicespec);

    s.opcode = OP_NET;
    s.unit = unit;
    s.cmd = 'S';
    s.aux1 = s.aux2 = 0;

    bus_ready();
    dwwrite((uint8_t *)&s, sizeof(s));
    status = net_get_response(unit, (uint8_t *)&sr, sizeof(sr));

    *bw = sr.bw;
    *c = sr.c;
    *err = sr.err;
    
    return status;
}

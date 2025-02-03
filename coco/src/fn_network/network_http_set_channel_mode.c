#include <cmoc.h>
#include <coco.h>
#include <dw.h>
#include <fujinet-fuji-coco.h>
#include <fujinet-network-coco.h>
#include "fujinet-network.h"

uint8_t network_http_set_channel_mode(const char *devicespec, uint8_t mode)
{
    struct _hscm
    {
        uint8_t opcode;
        uint8_t unit;
        uint8_t cmd;
        uint8_t mode;
        uint8_t unused;
    } hscm;

    hscm.opcode = OP_NET;
    hscm.cmd = 'M';
    hscm.unit = network_unit(devicespec);
    hscm.mode = mode;
    hscm.unused = 0;

    bus_ready();
    dwwrite((uint8_t *)&hscm, sizeof(hscm));
    
    return network_get_error(hscm.unit);
}

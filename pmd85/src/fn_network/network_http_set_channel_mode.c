#include <dw.h>
#include <fujinet-fuji-pmd85.h>
#include <fujinet-network-pmd85.h>
#include "fujinet-network.h"

uint8_t network_http_set_channel_mode(const char *devicespec, uint8_t mode)
{
    struct _hscm
    {
        uint8_t opcode;
        uint8_t unit;
        uint8_t cmd;
        uint8_t unused;
        uint8_t mode;
    } hscm;

    hscm.opcode = OP_NET;
    hscm.cmd = 'M';
    hscm.unit = network_unit(devicespec);
    hscm.unused = 0;
    hscm.mode = mode;

    bus_ready();
    dwwrite((uint8_t *)&hscm, sizeof(hscm));
    
    return network_get_error(hscm.unit);
}

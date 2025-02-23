#include <dw.h>
#include <fujinet-fuji-pmd85.h>
#include <fujinet-network-pmd85.h>
#include <fujinet-network.h>

uint8_t network_read_pmd85(const char* devicespec, uint8_t *buf, uint16_t len)
{
    uint8_t z = 0;

    struct _readcmd
    {
        uint8_t opcode;
        uint8_t id;
        uint8_t command;
        uint16_t len;
    } rc;

    uint8_t unit = network_unit(devicespec);

    rc.opcode = OP_NET;
    rc.id = unit;
    rc.command = 'R';
    rc.len = (len << 8) | (len >> 8);

    bus_ready();
    dwwrite((uint8_t *)&rc, sizeof(rc));
    return network_get_response(unit, buf, len);
}

#include <dw.h>
#include <fujinet-fuji-pmd85.h>
#include <fujinet-network.h>
#include <fujinet-network-pmd85.h>

uint8_t network_json_parse(const char *devicespec)
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
    jp.cmd = 0xFC;  // CMD_SET_CHANNEL_MODE;
    jp.aux1 = 1;    // CHANNELMODE_JSON
    jp.aux2 = 0;

    bus_ready();    
    dwwrite((uint8_t *)&jp, sizeof(jp));

    jp.cmd = 'P';
    jp.aux1 = jp.aux2 = 0;

    bus_ready();
    dwwrite((uint8_t *)&jp, sizeof(jp));

    return network_get_error(jp.unit);
}

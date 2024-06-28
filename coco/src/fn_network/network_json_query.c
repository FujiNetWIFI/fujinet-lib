#include <cmoc.h>
#include <coco.h>
#include <dw.h>
#include <fujinet-fuji-coco.h>
#include <fujinet-network.h>
#include <fujinet-network-coco.h>

int16_t network_json_query(char *devicespec, char *query, char *s)
{
    struct _jq
    {
        uint8_t opcode;
        uint8_t unit;
        uint8_t cmd;
        uint8_t aux1;
        uint8_t aux2;
        char query[256];
    } jq;

    jq.opcode = OP_NET;
    jq.unit = network_unit(devicespec);
    jq.cmd = 'Q';
    
    return bus_error(OP_NET) == BUS_SUCCESS;
}

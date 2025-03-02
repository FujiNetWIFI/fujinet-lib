#include <dw.h>
#include <fujinet-fuji-pmd85.h>
#include <fujinet-network.h>
#include <fujinet-network-pmd85.h>
#include <string.h>

int16_t network_json_query(const char *devicespec, const char *query, char *s)
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

    uint16_t bw=0;
    uint8_t c=0, err=0;
    uint8_t unit = network_unit(devicespec);

    jq.opcode = OP_NET;
    jq.unit = unit;
    jq.cmd = 'Q';
    jq.aux1 = jq.aux2 = 0;
    strcpy(jq.query, query);

    bus_ready();
    dwwrite((uint8_t *)&jq, sizeof(jq));

    network_status(devicespec, &bw, &c, &err);

    if (!bw)
        return 0;

    return network_read(devicespec, (uint8_t *)s, bw);
}

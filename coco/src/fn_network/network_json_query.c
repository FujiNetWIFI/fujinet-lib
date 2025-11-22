#include <cmoc.h>
#include <coco.h>
#include <dw.h>
#include <fujinet-fuji-coco.h>
#include <fujinet-network.h>
#include <fujinet-network-coco.h>

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
    char ch;
    int16_t offset = 0;
    int16_t read_len = 0;

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

    read_len =  network_read(devicespec, (uint8_t *)s, bw);

    if (read_len > 0)
    {
        // if last char is 0x9b, 0x0A or 0x0D, then set that char to nul, else just null terminate
        ch = s[read_len - 1];
        if (ch == 0x9B || ch == 0x0A || ch == 0x0D)
        {
            offset = 1;
        }
        s[read_len - offset] = '\0';
    }

    return read_len - offset;
}

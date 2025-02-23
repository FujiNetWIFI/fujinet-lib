#include "fujinet-fuji.h"
#include <dw.h>
#include <fujinet-fuji-pmd85.h>

bool fuji_read_directory(uint8_t maxlen, uint8_t aux2, char *buffer)
{
    struct _rd
    {
        uint8_t opcode;
        uint8_t cmd;
        uint8_t maxlen;
        uint8_t aux2;
    } rd;

    rd.opcode = OP_FUJI;
    rd.cmd = FUJICMD_READ_DIR_ENTRY;
    rd.maxlen = maxlen;
    rd.aux2 = aux2;

    bus_ready();

    dwwrite((uint8_t *)&rd, sizeof(rd));
    if (fuji_get_error())
        return false;
    
    return fuji_get_response((uint8_t *)buffer, maxlen);
}

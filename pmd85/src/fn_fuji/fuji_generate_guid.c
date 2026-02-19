#include "fujinet-fuji.h"
#include <dw.h>
#include <fujinet-fuji-pmd85.h>

bool fuji_generate_guid(char *buffer)
{
    struct _gg
    {
        uint8_t opcode;
        uint8_t cmd;
    } gg;

    gg.opcode = OP_FUJI;
    gg.cmd = FUJICMD_GENERATE_GUID;
    
    bus_ready();

    dwwrite((uint8_t *)&gg, sizeof(gg));
    if (fuji_get_error())
        return false;
    
    return fuji_get_response((uint8_t *)buffer, MAX_GUID_LEN);
}
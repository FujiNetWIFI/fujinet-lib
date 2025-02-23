#include "fujinet-fuji.h"
#include <dw.h>
#include <fujinet-fuji-pmd85.h>
#include <string.h>

bool fuji_open_directory(uint8_t hs, char *path_filter)
{
    struct _od
    {
        uint8_t opcode;
        uint8_t cmd;
        uint8_t hs;
        uint8_t path_filter[256];
    } od;

    od.opcode = OP_FUJI;
    od.cmd = FUJICMD_OPEN_DIRECTORY;
    od.hs = hs;
    memcpy(od.path_filter, path_filter, 256);

    bus_ready();
    dwwrite((uint8_t *)&od, sizeof(od));
    
    return !fuji_get_error();
}

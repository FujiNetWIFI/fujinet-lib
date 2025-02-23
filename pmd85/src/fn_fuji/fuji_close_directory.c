#include "fujinet-fuji.h"
#include <dw.h>
#include <fujinet-fuji-pmd85.h>

bool fuji_close_directory()
{
    struct _cd
    {
        uint8_t opcode;
        uint8_t cmd;
    } cd;

    cd.opcode = OP_FUJI;
    cd.cmd = FUJICMD_CLOSE_DIRECTORY;
    
    bus_ready();

    dwwrite((uint8_t *)&cd, sizeof(cd));
    
    return !fuji_get_error();
}

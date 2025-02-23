#include "fujinet-fuji.h"
#include <dw.h>
#include <fujinet-fuji-pmd85.h>

bool fuji_get_directory_position(uint16_t *pos)
{
    struct _gdp
    {
        uint8_t opcode;
        uint8_t cmd;
    } gdp;

    gdp.opcode = OP_FUJI;
    gdp.cmd = FUJICMD_GET_DIRECTORY_POSITION;

    bus_ready();

    dwwrite((uint8_t *)&gdp, sizeof(gdp));
    if (fuji_get_error())
        return false;
    
    return fuji_get_response((uint8_t *)pos, sizeof(uint16_t));
}

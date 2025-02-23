#include "fujinet-fuji.h"
#include <dw.h>
#include <fujinet-fuji-pmd85.h>

bool fuji_get_device_filename(uint8_t ds, char *buffer)
{
    struct _gdf
    {
        uint8_t opcode;
        uint8_t cmd;
        uint8_t ds;
    } gdf;

    gdf.opcode = OP_FUJI;
    gdf.cmd = FUJICMD_GET_DEVICE_FULLPATH;
    gdf.ds = ds;

    bus_ready();

    dwwrite((uint8_t *)&gdf, sizeof(gdf));
    if (fuji_get_error())
        return false;
    
    return fuji_get_response((uint8_t *)buffer, 256);
}

#include <cmoc.h>
#include <coco.h>
#include "fujinet-fuji.h"
#include <dw.h>
#include <fujinet-fuji-coco.h>

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
    dwread((uint8_t *)buffer, 256);
    
    return bus_error(OP_FUJI) == BUS_SUCCESS;
}

#include <cmoc.h>
#include <coco.h>
#include "fujinet-fuji.h"
#include <dw.h>
#include <fujinet-fuji-coco.h>

bool fuji_set_device_filename(uint8_t mode, uint8_t hs, uint8_t ds, char *buffer)
{
    struct _sdf
    {
        uint8_t opcode;
        uint8_t cmd;
        uint8_t ds;
        uint8_t hs;
        uint8_t mode;
        char filename[256];
    } sdf;

    sdf.opcode = OP_FUJI;
    sdf.cmd = FUJICMD_SET_DEVICE_FULLPATH;
    sdf.ds = ds;
    sdf.hs = hs;
    sdf.mode = mode;
    strcpy(sdf.filename,buffer);

    bus_ready();
    dwwrite((uint8_t *)&sdf, sizeof(sdf));

    return !fuji_get_error();
}

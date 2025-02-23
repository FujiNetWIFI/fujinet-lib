#include "fujinet-fuji.h"
#include <dw.h>
#include <fujinet-fuji-pmd85.h>

bool fuji_mount_disk_image(uint8_t ds, uint8_t mode)
{
    struct _mdi
    {
        uint8_t opcode;
        uint8_t cmd;
        uint8_t ds;
        uint8_t mode;
    } mdi;

    mdi.opcode = OP_FUJI;
    mdi.cmd = FUJICMD_MOUNT_IMAGE;
    mdi.ds = ds;
    mdi.mode = mode;
    
    bus_ready();
    dwwrite((uint8_t *)&mdi, sizeof(mdi));
    
    return !fuji_get_error();
}

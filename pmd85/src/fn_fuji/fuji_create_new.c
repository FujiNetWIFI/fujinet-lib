#include "fujinet-fuji.h"
#include <dw.h>
#include <fujinet-fuji-pmd85.h>
#include <string.h>

bool fuji_create_new(NewDisk *new_disk)
{
    struct _cn
    {
        uint8_t opcode;
        uint8_t cmd;
        uint8_t numDisks;
        uint8_t hostSlot;
        uint8_t deviceSlot;
        char filename[256];
    } cn;

    cn.opcode = OP_FUJI;
    cn.cmd = FUJICMD_NEW_DISK;
    cn.numDisks = new_disk->numDisks;
    cn.hostSlot = new_disk->hostSlot;
    cn.deviceSlot = new_disk->deviceSlot;
    strcpy(cn.filename, new_disk->filename);

    bus_ready();

    dwwrite((uint8_t *)&cn, sizeof(cn));

    return !fuji_get_error();
}

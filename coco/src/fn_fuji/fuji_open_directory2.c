#include <cmoc.h>
#include <coco.h>
#include "fujinet-fuji.h"
#include <dw.h>
#include <fujinet-fuji-coco.h>

bool fuji_open_directory2(uint8_t hs, char *path, char *filter)
{
    struct _od2
    {
        uint8_t opcode;
        uint8_t cmd;
        uint8_t hs;
        char path[256];
    } od2;

    od2.opcode = OP_FUJI;
    od2.cmd = FUJICMD_OPEN_DIRECTORY;
    od2.hs = hs;

    strcpy(od2.path,path);
    strcpy(&od2.path[strlen(od2.path)+1],filter);

    bus_ready();

    dwwrite((uint8_t *)&od2,sizeof(od2));
    
    return !fuji_get_error();
}

#include "fujinet-fuji.h"
#include <dw.h>
#include <fujinet-fuji-pmd85.h>

bool fuji_set_boot_mode(uint8_t mode)
{
    struct _sbm
    {
        uint8_t opcode;
        uint8_t cmd;
        uint8_t mode;
    } sbm;

    sbm.opcode = OP_FUJI;
    sbm.cmd = FUJICMD_SET_BOOT_MODE;
    sbm.mode = mode;

    bus_ready();
    dwwrite((uint8_t *)&sbm, sizeof(sbm));
    
    return !fuji_get_error();
}

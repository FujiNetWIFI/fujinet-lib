#include <cmoc.h>
#include <coco.h>
#include "fujinet-fuji.h"
#include <dw.h>
#include <fujinet-fuji-coco.h>

bool fuji_set_boot_mode(uint8_t mode)
{
    struct _sbm
    {
        uint8_t opcode;
        uint8_t cmd;
        uint8_t mode;
    } sbm;

    bus_ready();
    dwwrite((uint8_t *)&sbm, sizeof(sbm));
    
    return bus_error(OP_FUJI) == BUS_SUCCESS;
}

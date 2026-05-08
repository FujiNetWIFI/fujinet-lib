#include <cmoc.h>
#include <coco.h>
#include "fujinet-fuji.h"
#include <dw.h>
#include <fujinet-fuji-coco.h>

bool fuji_copy_file(uint8_t src_slot, uint8_t dst_slot, char *copy_spec)
{
    struct _cf
    {
        uint8_t opcode;
        uint8_t cmd;
        uint8_t src_slot;
        uint8_t dst_slot;
        char copy_spec[256];
    } cf;

    cf.opcode = OP_FUJI;
    cf.cmd = FUJICMD_COPY_FILE;
    cf.src_slot = src_slot;
    cf.dst_slot = dst_slot;
    strcpy(cf.copy_spec, copy_spec);
    
    bus_ready();

    dwwrite((uint8_t *)&cf, sizeof(cf));

    return !fuji_get_error();
}

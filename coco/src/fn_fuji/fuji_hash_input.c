#include <cmoc.h>
#include <coco.h>
#include "fujinet-fuji.h"
#include <dw.h>
#include <fujinet-fuji-coco.h>

bool fuji_hash_input(char *s, uint16_t len)
{
    struct _hi
    {
        uint8_t opcode;
        uint8_t cmd;
        uint16_t len;
    } hi;

    hi.opcode = OP_FUJI;
    hi.cmd = FUJICMD_HASH_INPUT;
    hi.len = len;
    
    bus_ready();
    dwwrite((uint8_t *)&hi, sizeof(hi));
    dwwrite((uint8_t *)s, len);
    
    return !fuji_get_error();
}

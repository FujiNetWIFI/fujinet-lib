#include <cmoc.h>
#include <coco.h>
#include "fujinet-fuji.h"
#include <dw.h>
#include <fujinet-fuji-coco.h>

bool fuji_read_appkey(uint8_t key_id, uint16_t *count, uint8_t *data)
{
    struct _ra
    {
        uint8_t opcode;
        uint8_t cmd;
        uint8_t key_id;
    } ra;

    ra.opcode = OP_FUJI;
    ra.cmd = FUJICMD_READ_APPKEY;
    ra.key_id = key_id;

    bus_ready();
    
    dwwrite((uint8_t *)&ra, sizeof(ra));
    // TODO Fix this API, it's broken. shit.
    
    return bus_error(OP_FUJI) == BUS_SUCCESS;
}

#include <cmoc.h>
#include <coco.h>
#include "fujinet-fuji.h"
#include <fujinet-fuji-coco.h>
#include <dw.h>

bool fuji_base64_decode_compute()
{

    struct _fbdc
    {
        uint8_t opcode;
        uint8_t cmd;
    } fbdc;

    fbdc.opcode = OP_FUJI;
    fbdc.cmd = FUJICMD_BASE64_DECODE_COMPUTE;

    bus_ready();
    dwwrite((uint8_t *)&fbdc, sizeof(fbdc));
    
    return bus_error(OP_FUJI) == BUS_SUCCESS;
}

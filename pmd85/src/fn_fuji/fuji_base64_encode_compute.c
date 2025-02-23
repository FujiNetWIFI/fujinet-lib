#include "fujinet-fuji.h"
#include <dw.h>
#include <fujinet-fuji-pmd85.h>

bool fuji_base64_encode_compute()
{
    struct _bec
    {
        uint8_t opcode;
        uint8_t cmd;
    } bec;

    bec.opcode = OP_FUJI;
    bec.cmd = FUJICMD_BASE64_ENCODE_COMPUTE;
    
    bus_ready();

    dwwrite((uint8_t *)&bec, sizeof(bec));
    
    return !fuji_get_error();
}

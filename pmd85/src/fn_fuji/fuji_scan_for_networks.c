#include "fujinet-fuji.h"
#include <dw.h>
#include <fujinet-fuji-pmd85.h>

bool fuji_scan_for_networks(uint8_t *count)
{
    struct _sfn
    {
        uint8_t opcode;
        uint8_t cmd;
    } sfn;

    sfn.opcode = OP_FUJI;
    sfn.cmd = FUJICMD_SCAN_NETWORKS;

    bus_ready();
    dwwrite((uint8_t *)&sfn, sizeof(sfn));
    if (fuji_get_error())
        return false;
    
    return fuji_get_response(count, sizeof(uint8_t));
}

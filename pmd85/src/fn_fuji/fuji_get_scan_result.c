#include "fujinet-fuji.h"
#include <dw.h>
#include <fujinet-fuji-pmd85.h>

bool fuji_get_scan_result(uint8_t n, SSIDInfo *ssid_info)
{
    struct _gsr
    {
        uint8_t opcode;
        uint8_t cmd;
        uint8_t n;
    } gsr;

    gsr.opcode = OP_FUJI;
    gsr.cmd = FUJICMD_GET_SCAN_RESULT;
    gsr.n = n;

    bus_ready();

    dwwrite((uint8_t *)&gsr, sizeof(gsr));
    if (fuji_get_error())
        return false;
    
    return fuji_get_response((uint8_t *)ssid_info, sizeof(SSIDInfo));
}

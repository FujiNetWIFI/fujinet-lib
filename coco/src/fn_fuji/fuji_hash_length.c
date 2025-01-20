#include <cmoc.h>
#include <coco.h>
#include "fujinet-fuji.h"
#include <dw.h>
#include <fujinet-fuji-coco.h>

// This function is broken in the API (no capability to get the response value), use \ref fuji_hash_size instead
bool fuji_hash_length(uint8_t mode)
{
    return false;
    // uint8_t len = 0;
    
    // struct _hl
    // {
    //     uint8_t opcode;
    //     uint8_t cmd;
    //     uint8_t mode;
    // } hl;

    // hl.opcode = OP_FUJI;
    // hl.cmd = FUJICMD_HASH_LENGTH;
    // hl.mode = mode;

    // bus_ready();

    // dwwrite((uint8_t *)&hl, sizeof(hl));
    // fuji_get_response(&len, sizeof(uint8_t));
    
    // return bus_error(OP_FUJI) == BUS_SUCCESS;
}

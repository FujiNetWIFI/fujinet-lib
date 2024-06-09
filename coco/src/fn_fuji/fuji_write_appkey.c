#include <cmoc.h>
#include <coco.h>
#include "fujinet-fuji.h"
#include <dw.h>
#include <fujinet-fuji-coco.h>

bool fuji_write_appkey(uint8_t key_id, uint16_t count, uint8_t *data)
{
    // I need to talk with fenrock on why the api for appkeys is like this
    return bus_error(OP_FUJI) == BUS_SUCCESS;
}

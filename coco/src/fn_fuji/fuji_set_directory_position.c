#include <cmoc.h>
#include <coco.h>
#include "fujinet-fuji.h"
#include <dw.h>
#include <fujinet-fuji-coco.h>

bool fuji_set_directory_position(uint16_t pos)
{
    
    return bus_error(OP_FUJI) == BUS_SUCCESS;
}

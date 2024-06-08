#include <cmoc.h>
#include <coco.h>
#include "fujinet-fuji.h"
#include <dw.h>
#include <fujinet-fuji-coco.h>

bool fuji_error()
{
    return bus_error(OP_FUJI) == BUS_SUCCESS;
}

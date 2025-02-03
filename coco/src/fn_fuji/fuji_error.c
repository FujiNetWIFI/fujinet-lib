#include <cmoc.h>
#include <coco.h>
#include "fujinet-fuji.h"
#include "fujinet-network.h"
#include "fujinet-fuji-coco.h"

bool fuji_error(void)
{
    return fn_device_error != BUS_SUCCESS;
}

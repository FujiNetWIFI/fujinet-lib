#include "fujinet-fuji.h"
#include "fujinet-network.h"
#include "fujinet-fuji-pmd85.h"

bool fuji_error(void)
{
    return fn_device_error != BUS_SUCCESS;
}

#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-msdos.h"

bool fuji_get_hsio_index(uint8_t *index)
{
    *index = 1;
    return true;
}

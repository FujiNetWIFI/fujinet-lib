#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-msdos.h"

bool fuji_reset(void)
{
    return int_f5(0x70,0xFF,0x00,0x00) == 'C';
}

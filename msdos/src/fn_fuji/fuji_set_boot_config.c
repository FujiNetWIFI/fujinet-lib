#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-msdos.h"

bool fuji_set_boot_config(uint8_t toggle)
{
    return int_f5_ah_00(0x70,0xD9,toggle,0x00) == 'C';
}

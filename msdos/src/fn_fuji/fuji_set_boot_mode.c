#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-msdos.h"

bool fuji_set_boot_mode(uint8_t mode)
{
    return int_f5_ah_00(0x70,0xD6,mode,0x00) == 'C';
}

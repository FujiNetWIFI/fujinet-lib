#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-msdos.h"

bool fuji_close_directory(void)
{
    return int_f5_ah_00(0x70,0xF5,0x00,0x00) == 'C';
}

#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-msdos.h"

bool fuji_set_directory_position(uint16_t pos)
{
    return int_f5_ah_00(0x70,0xE4,pos&0xFF,pos>>8) == 'C';
}

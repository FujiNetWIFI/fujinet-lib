#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-msdos.h"

bool fuji_get_directory_position(uint16_t *pos)
{
    return int_f5_ah_40(0x70,0xE5,0x00,0x00,pos,sizeof(uint16_t)) == 'C';
}

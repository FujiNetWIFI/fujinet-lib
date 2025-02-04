#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-msdos.h"

bool fuji_hash_compute(uint8_t type)
{
    return int_f5_ah_00(0x70,0xC7,type,0x00) == 'C';
}

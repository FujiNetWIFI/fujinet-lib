#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-msdos.h"

bool fuji_hash_compute_no_clear(uint8_t type)
{
    return int_f5(0x70,0xC3,type,0x00) == 'C';
}

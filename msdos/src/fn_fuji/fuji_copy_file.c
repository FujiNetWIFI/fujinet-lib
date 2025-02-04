#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-msdos.h"

bool fuji_copy_file(uint8_t src_slot, uint8_t dst_slot, char *copy_spec)
{
    return int_f5_ah_80(0x70,0xD8,src_slot,dst_slot,copy_spec,256) == 'C';
}

#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-msdos.h"

bool fuji_get_host_prefix(uint8_t hs, char *prefix)
{
    return int_f5_ah_40(0x70,0xE0,hs,0x00,prefix,256) == 'C';
}

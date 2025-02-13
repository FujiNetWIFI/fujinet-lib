#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-msdos.h"

bool fuji_scan_for_networks(uint8_t *count)
{
    return int_f5_read(0x70,0xFD,0x00,0x00,count,1) == 'C';
}

#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-msdos.h"

bool fuji_mount_host_slot(uint8_t hs)
{
    return int_f5_ah_00(0x70,0xF9,hs,0x00) == 'C';
}

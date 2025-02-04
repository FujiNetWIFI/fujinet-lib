#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-msdos.h"

bool fuji_mount_disk_image(uint8_t ds, uint8_t mode)
{
    return int_f5_ah_00(0x70,0xF8,ds,mode) == 'C';
}

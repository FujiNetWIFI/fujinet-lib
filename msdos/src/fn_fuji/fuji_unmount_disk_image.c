#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-msdos.h"

bool fuji_unmount_disk_image(uint8_t ds)
{
    return int_f5(0x70,0xE9,ds,0x00) == 'C';
}

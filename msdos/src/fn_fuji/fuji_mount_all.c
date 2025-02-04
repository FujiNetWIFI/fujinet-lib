#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-msdos.h"

bool fuji_mount_all(void)
{
    return int_f5_ah_00(0x70,0xD7,0x00,0x00) == 'C';
}

#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-msdos.h"

bool fuji_get_host_slots(HostSlot *h, size_t size)
{
    return int_f5_read(0x70,0xF4,0x00,0x00,h,size) == 'C';
}

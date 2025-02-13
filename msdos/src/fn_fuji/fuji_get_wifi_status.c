#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-msdos.h"

bool fuji_get_wifi_status(uint8_t *status)
{
    return int_f5_read(0x70,0xFA,0x00,0x00,status,sizeof(uint8_t)) == 'C';
}

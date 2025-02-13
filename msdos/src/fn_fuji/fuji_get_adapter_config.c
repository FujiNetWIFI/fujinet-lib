#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-msdos.h"

bool fuji_get_adapter_config(AdapterConfig *ac)
{
    return int_f5_read(0x70,0xE8,0x00,0x00,ac,sizeof(AdapterConfig)) == 'C';
}

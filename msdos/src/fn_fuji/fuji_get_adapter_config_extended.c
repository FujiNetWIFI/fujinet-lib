#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-msdos.h"

bool fuji_get_adapter_config_extended(AdapterConfigExtended *ac)
{
    return int_f5_ah_40(0x70,0xC4,0x00,0x00,ac,sizeof(AdapterConfigExtended)) == 'C';
}

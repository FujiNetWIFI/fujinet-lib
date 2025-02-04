#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-msdos.h"

bool fuji_get_wifi_enabled(void)
{
    uint8_t enabled = false;

    if (int_f5_ah_40(0x70,0xEA,0x00,0x00,(void *)&enabled,sizeof(uint8_t)) == 'C')
        return enabled;

    return false;
}

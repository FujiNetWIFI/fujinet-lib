#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-msdos.h"

bool fuji_get_ssid(NetConfig *net_config)
{
    return int_f5_ah_40(0x70,0xFE,0x00,0x00,net_config,sizeof(NetConfig)) == 'C';
}

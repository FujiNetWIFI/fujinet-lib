#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-msdos.h"

bool fuji_set_ssid(NetConfig *nc)
{
    return int_f5_ah_80(0x70,0xFB,0x00,0x00,nc,sizeof(NetConfig)) == 'C';
}

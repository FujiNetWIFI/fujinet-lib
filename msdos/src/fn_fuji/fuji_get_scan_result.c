#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-msdos.h"

bool fuji_get_scan_result(uint8_t n, SSIDInfo *ssid_info)
{
    return int_f5_ah_40(0x70,0xFC,n,0x00,ssid_info,sizeof(SSIDInfo)) == 'C';
}

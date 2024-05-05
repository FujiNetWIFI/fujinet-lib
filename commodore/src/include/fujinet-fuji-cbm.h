#ifndef FUJINET_FUJI_CBM_H
#define FUJINET_FUJI_CBM_H

#include <stdint.h>

uint8_t __fastcall__ fuji_cbm_open(uint8_t lfn, uint8_t device, uint8_t sec_addr, uint8_t len, uint8_t* name);

#endif // FUJINET_FUJI_CBM_H
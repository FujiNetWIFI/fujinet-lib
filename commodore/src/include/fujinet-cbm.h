#ifndef FUJINET_CBM_H
#define FUJINET_CBM_H

// COMMON FILE FOR ALL FUJI/NETWORK CODE FOR CBM

#include <cbm.h>

#include "fujinet-pet-charmap.h"

#define CBM_DEV_FUJI          30
#define CBM_DEV_NETWORK       16

#define CBM_CMD_CHANNEL       15
#define CBM_DATA_CHANNEL      2
// zero based version of above to avoid an addition
#define CBM_DATA_CHANNEL_0    1

// Our custom cbm_open that works with binary data by not doing strlen on the name, but instead takes a length parameter
uint8_t fuji_cbm_open(uint8_t lfn, uint8_t device, uint8_t sec_addr, uint8_t len, uint8_t* name);

#endif // FUJINET_CBM_H
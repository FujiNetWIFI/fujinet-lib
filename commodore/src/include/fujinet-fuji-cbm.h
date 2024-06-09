#ifndef FUJINET_FUJI_CBM_H
#define FUJINET_FUJI_CBM_H

#include <stdbool.h>
#include <stdint.h>

// Our custom cbm_open that works with binary data by not doing strlen on the name, but instead takes a length parameter
uint8_t __fastcall__ fuji_cbm_open(uint8_t lfn, uint8_t device, uint8_t sec_addr, uint8_t len, uint8_t* name);

// helper that just does an open/close with the given data, used in many simple functions
bool __fastcall__ open_close(uint8_t size, uint8_t *data);

// helper that just does an open/read/close returning data via in, setup data is in out
bool open_read_close(uint8_t out_size, uint8_t *out, uint16_t in_size, uint8_t *in);

#endif // FUJINET_FUJI_CBM_H
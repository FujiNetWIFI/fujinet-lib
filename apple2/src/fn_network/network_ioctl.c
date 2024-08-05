#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "fujinet-network.h"
#include "fujinet-bus-apple2.h"

extern uint8_t __argsize__;
extern uint8_t bad_unit();

// uint8_t network_ioctl(uint8_t cmd, uint8_t aux1, uint8_t aux2, char* devicespec, int16_t use_aux, void *buffer, uint16_t len);
//
// if use_aux is true (1), we put aux1/2 into payload[2,3], and any buffer will copy len bytes - 2 after it to payload[4+].
// if use_aux is false, copy buffer to payload[2+]
// if buffer is NULL, no copying ever done from buffer
// len is set at payload[0,1]

uint8_t network_ioctl(uint8_t cmd, uint8_t aux1, uint8_t aux2, char* devicespec, ...) {
    va_list args;
	uint8_t nw_device;
    uint16_t len;
    void *buffer;
    bool use_aux;
    uint8_t offset = 2;

    // we expect __argsize__ to be 11 from:
    //   5 for fixed args (u8 * 3 + char *)
    //  +6 (3 args, 2 bytes each)
    // otherwise the function was called with wrong number of args
    if (__argsize__ != 11) {
        return fn_error(SP_ERR_BAD_CMD);
    }

	if (sp_is_init == 0) {
        nw_device = sp_init();
        if (nw_device == 0) {
            return fn_error(SP_ERR_IO_ERROR);
        }
    }

	if (sp_network == 0) {
        return bad_unit();
	}

    // read the args. Note the first values are in order args are passed, unlike asm which is from right backwards
    // every vararg takes 2 bytes, even bool/uint8_t, so fetch them in pairs
    va_start(args, devicespec);
    use_aux = (bool) va_arg(args, int);
    buffer = (void *) va_arg(args, int);
    len = (uint16_t) va_arg(args, int);
    va_end(args);

    // find the Nx: unit id
    sp_nw_unit = network_unit(devicespec);

    // construct the payload
    sp_clr_payload();
    sp_payload[0] = len & 0xFF;
    sp_payload[1] = len >> 8;

    if (use_aux) {
        sp_payload[2] = aux1;
        sp_payload[3] = aux2;
        offset = 4;
    }

    if (buffer != NULL) {
        // copy into sp_payload[2] or [4] depending on use_aux. Only copy len or len-2 bytes.
        memcpy(sp_payload + offset, buffer, len - offset + 2);
    }

    return fn_error(sp_control_nw(sp_network, cmd));
}
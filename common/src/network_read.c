#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "../../fujinet-network.h"

#ifdef BUILD_ATARI
#include "../../fujinet-lib-atari.h"
#endif

#ifdef BUILD_APPLE2
#include "../../fujinet-lib-apple2.h"
#include "../../apple2/src/fn_fuji/inc/sp.h"
#endif

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

#define MAX_READ_SIZE 512


uint8_t network_read(char *devicespec, uint8_t *buf, uint16_t len)
{
    uint8_t r = 0;
    uint16_t fetch_size = 0;
    uint16_t amount_left = len;
    uint16_t total_read = 0;
#ifdef BUILD_ATARI
    uint8_t unit = 0;
#endif

    if (len == 0 || buf == NULL) {
#ifdef BUILD_ATARI
        return fn_error(132); // invalid command
#endif
#ifdef BUILD_APPLE2
        return fn_error(SP_ERR_BAD_CMD);
#endif
    }

#ifdef BUILD_APPLE2
    // check we have the SP network value
    if (sp_network == 0) {
        return fn_error(SP_ERR_BAD_UNIT);
    }
#endif

    fn_bytes_read = 0;
    fn_device_error = 0;

#ifdef BUILD_ATARI
    unit = network_unit(devicespec);
#endif


    while (1) {
        // exit condition
        if (amount_left == 0) break;

#ifdef BUILD_ATARI
        r = network_status_unit(unit, &fn_network_bw, &fn_network_conn, &fn_network_error);
#endif
#ifdef BUILD_APPLE2
        r = network_status_no_clr(devicespec, &fn_network_bw, &fn_network_conn, &fn_network_error);
#endif

        if (r != 0) return r;

        // EOF hit, exit reading
        if (fn_network_error == 136) break;

        // we are waiting for bytes to become available while still connected. Causes tight loop if there's a long delay reading from network into FN
        if (fn_network_bw == 0 && fn_network_conn == 1) {
            continue;
        }

        fetch_size = MIN(amount_left, fn_network_bw);

#ifdef BUILD_APPLE2
        // need to validate this is only required for apple
        fetch_size = MIN(fetch_size, MAX_READ_SIZE);
#endif

#ifdef BUILD_ATARI
        sio_read(unit, buf, fetch_size);
#endif

#ifdef BUILD_APPLE2
        sp_read(sp_network, fetch_size);
        memcpy(buf, sp_payload, fetch_size);
#endif

        buf += fetch_size;
        amount_left -= fetch_size;
        total_read += fetch_size;
    }

    // do this here at the end, not in the loop so sio_read for atari can continue to set fn_bytes_read for short reads.
    fn_bytes_read = total_read;
    return 0;
}
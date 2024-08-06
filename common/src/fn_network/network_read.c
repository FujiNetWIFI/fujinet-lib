#ifdef _CMOC_VERSION_
#include <cmoc.h>
#include <coco.h>
#include <fujinet-network-coco.h>
#include <fujinet-fuji-coco.h>
#include <dw.h>
#else
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#endif /* _CMOC_VERSION_ */
#include "fujinet-network.h"

#ifdef __ATARI__
#include "fujinet-network-atari.h"
#include "fujinet-bus-atari.h"
#endif

#ifdef __APPLE2__
#include "fujinet-network-apple2.h"
#include "fujinet-bus-apple2.h"
#include "sp.h"
#endif

#ifdef __CBM__
#include "fujinet-network-cbm.h"
#endif

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

#define MAX_READ_SIZE 512


int16_t network_read(char *devicespec, uint8_t *buf, uint16_t len)
{
    uint8_t r = 0;
    uint16_t fetch_size = 0;
    uint16_t amount_left = len;
    uint16_t total_read = 0;

#if defined(_CMOC_VERSION_)
    struct _r
    {
        uint8_t opcode;
        uint8_t unit;
        uint8_t cmd;
        uint16_t len;
    } read_r;
#endif

#if defined(__CBM__)
    const char *after;
#endif

#if defined(__ATARI__) || defined(_CMOC_VERSION_) || defined(__CBM__)
    uint8_t unit = 0;
#endif

    if (len == 0 || buf == NULL) {

#if defined(__ATARI__)
        return fn_error(132); // invalid command
#elif defined(__APPLE2__)
        return fn_error(SP_ERR_BAD_CMD);
#elif defined(__CBM__)
        return FN_ERR_BAD_CMD;
#elif defined(_CMOC_VERSION_)
        return fn_error(132); // invalid command
#endif

    }

#ifdef __APPLE2__
    // check we have the SP network value
    if (sp_network == 0) {
        return fn_error(SP_ERR_BAD_UNIT);
    }
#endif

    fn_bytes_read = 0;
    fn_device_error = 0;

#if defined(__ATARI__) || defined(_CMOC_VERSION_)
    unit = network_unit(devicespec);
#elif defined(__CBM__)
    unit = getDeviceNumber(devicespec, &after);
#endif

    while (1) {
        // exit condition
        if (amount_left == 0) break;

#if defined(__ATARI__)
        r = network_status_unit(unit, &fn_network_bw, &fn_network_conn, &fn_network_error);
#elif defined(__APPLE2__)
        r = network_status(devicespec, &fn_network_bw, &fn_network_conn, &fn_network_error);
#elif defined(__CBM__)
        r = network_status(devicespec, &fn_network_bw, &fn_network_conn, &fn_network_error);
#elif defined(_CMOC_VERSION_)
        r = network_status(devicespec, &fn_network_bw, &fn_network_conn, &fn_network_error); // TODO: Status return needs fixing.
#endif

        // check if the status failed
        if (r != 0) return -r;

        // EOF hit, exit reading
        if (fn_network_error == 136) break;

        // we are waiting for bytes to become available while still connected.
        // Causes tight loop if there's a long delay reading from network into FN, so we may see lots of status requests
        if (fn_network_bw == 0 && fn_network_conn == 1) {
            continue;
        }

        fetch_size = MIN(amount_left, fn_network_bw);
#ifdef __APPLE2__
        // need to validate this is only required for apple
        fetch_size = MIN(fetch_size, MAX_READ_SIZE);
#endif

#if defined(__ATARI__)
        sio_read(unit, buf, fetch_size);
#elif defined(__APPLE2__)
        sp_read(sp_network, fetch_size);
        memcpy(buf, sp_payload, fetch_size);
#elif defined(__CBM__)
        cbm_read(unit + CBM_DATA_CHANNEL_0, buf, fetch_size);
#elif defined(_CMOC_VERSION_)
        read_r.opcode = OP_NET;
        read_r.unit = unit;
        read_r.cmd = 'R';
        read_r.len = fetch_size;

        bus_ready();
        dwwrite((uint8_t *)&read_r, sizeof(read_r));
        bus_get_response(OP_NET, (uint8_t *)buf, fetch_size);
#endif
        
        buf += fetch_size;
        amount_left -= fetch_size;
        total_read += fetch_size;
    }

    // do this here at the end, not in the loop so sio_read for atari can continue to set fn_bytes_read for short reads.
    fn_bytes_read = total_read;
    return total_read;
}

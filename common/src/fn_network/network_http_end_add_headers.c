#ifdef _CMOC_VERSION_
#include <cmoc.h>
#else
#include <stdint.h>
#endif /* _CMOC_VERSION_ */
#include "fujinet-network.h"

uint8_t network_http_end_add_headers(const char *devicespec) {
    return network_http_set_channel_mode(devicespec, HTTP_CHAN_MODE_BODY);
}

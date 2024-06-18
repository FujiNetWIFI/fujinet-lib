#ifdef _CMOC_VERSION_
#include <cmoc.h>
#else
#include <stdint.h>
#endif /* _CMOC_VERSION_ */
#include "fujinet-network.h"

uint8_t network_http_put(char *devicespec, char *data) {
    return network_http_post(devicespec, data);
}

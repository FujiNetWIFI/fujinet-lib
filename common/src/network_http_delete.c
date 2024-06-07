#ifdef _CMOC_VERSION_
#include <cmoc.h>
#else
#include <stdint.h>
#endif /* _CMOC_VERSION_ */
#include "fujinet-network.h"

uint8_t network_http_delete(char *devicespec, uint8_t trans) {
    return network_open(devicespec, OPEN_MODE_HTTP_DELETE_H, trans);
}

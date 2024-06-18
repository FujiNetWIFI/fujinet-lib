#ifdef _CMOC_VERSION_
#include <cmoc.h>
#else
#include <stdint.h>
#include <string.h>
#endif /* _CMOC_VERSION_ */
#include "fujinet-network.h"

uint8_t network_http_add_header(char *devicespec, char *header) {
    return network_write(devicespec, (uint8_t *) header, strlen(header));
}

#ifdef _CMOC_VERSION_

#include <cmoc.h>
#include <coco.h>

#else

#include <stdint.h>

#endif /* _CMOC_VERSION_ */

/**
 * @brief return network unit number given devicespec string
 * @param devicespec A string containing device spec.
 * @return Unit number, 0=invalid, >0=valid.
 */
uint8_t network_unit(char *devicespec)
{
    if (devicespec[1] == ':')
        return 1;

    if (devicespec[2] == ':') {
        return devicespec[1] - '0';
    }

    return 1;
}


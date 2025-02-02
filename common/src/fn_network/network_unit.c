#ifdef _CMOC_VERSION_

#include <cmoc.h>
#include <coco.h>

#else

#include <stdint.h>

#endif /* _CMOC_VERSION_ */

/**
 * @brief return network unit number given devicespec string
 * @param devicespec A string containing device spec.
 * @return Unit number, 1 if nothing specified (e.g. "n:", "x:", "foo"), or the given value (char - '0') if specified (e.g. "n2:", "n9:").
 * Note there is limited checking here, if you specify "na:" you will get very odd results. 
 */
uint8_t network_unit(const char *devicespec)
{
    if (devicespec[1] == ':')
        return 1;

    if (devicespec[2] == ':') {
        return devicespec[1] - '0';
    }

    return 1;
}


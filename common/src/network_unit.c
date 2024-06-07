#ifdef _CMOC_VERSION_

#include <cmoc.h>
#include <coco.h>

/**
 * @brief return network unit number given devicespec string
 * @param devicespec A string containing device spec.
 * @return Unit number, 0=invalid, >0=valid.
 */
uint8_t network_unit(char *devicespec)
{
    if (devicespec[0]==0)
        return 0;

    if (devicespec[1]==':')
        return 1;

    if (devicespec[2]!=':')
        return 0;
    
    return devicespec[1];
}

#endif /* _CMOC_VERSION_ */

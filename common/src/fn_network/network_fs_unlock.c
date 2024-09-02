/**
 * @brief   Unlock file (make read/write) specified by N: devicespec
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 */

#ifdef _CMOC_VERSION_
#include <cmoc.h>
#else
#include <stdint.h>
#endif /* _CMOC_VERSION_ */
#include "fujinet-network.h"

uint8_t network_fs_unlock(char *devicespec) {
    return network_ioctl('$',0,0,devicespec);
}

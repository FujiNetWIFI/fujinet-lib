/**
 * @brief   Rename file specified by N: devicespec
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose N:TNFS://TMA-2/foo.txt,bar.txt
 */

#ifdef _CMOC_VERSION_
#include <cmoc.h>
#else
#include <stdint.h>
#endif /* _CMOC_VERSION_ */
#include "fujinet-network.h"

uint8_t network_fs_rename(char *devicespec) {
    return network_ioctl(' ',0,0,devicespec);
}

/**
 * @brief   Internal header for MS-DOS version of fujinet-lib
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 */

#ifndef FUJINET_FUJI_MSDOS_H
#define FUJINET_FUJI_MSDOS_H

/**
 * @brief bus functions.
 */
unsigned char int_f5_ah_00(unsigned char dev, unsigned char command, unsigned char aux1, unsigned char aux2);
unsigned char int_f5_ah_40(unsigned char dev, unsigned char command, unsigned char aux1, unsigned char aux2, void *buf, unsigned short len);
unsigned char int_f5_ah_80(unsigned char dev, unsigned char command, unsigned char aux1, unsigned char aux2, void *buf, unsigned short len);

#endif /* FUJINET_FUJI_MSDOS_H */

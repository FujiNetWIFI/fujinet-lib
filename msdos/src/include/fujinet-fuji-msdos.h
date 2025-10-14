/**
 * @brief   Internal header for MS-DOS version of fujinet-lib
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 */

#ifndef FUJINET_FUJI_MSDOS_H
#define FUJINET_FUJI_MSDOS_H

#define BUS_SUCCESS 0

/**
 * @brief bus functions.
 */
unsigned char int_f5(unsigned char dev, unsigned char command, unsigned char aux1, unsigned char aux2);
unsigned char int_f5_read(unsigned char dev, unsigned char command, unsigned char aux1, unsigned char aux2, void *buf, unsigned short len);
unsigned char int_f5_write(unsigned char dev, unsigned char command, unsigned char aux1, unsigned char aux2, const void *buf, unsigned short len);

#endif /* FUJINET_FUJI_MSDOS_H */

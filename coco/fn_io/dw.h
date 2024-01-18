#ifndef DW_H
#define DW_H

#include <cmoc.h>
#include <coco.h>

/**
 * @brief Read string to s from DriveWire with expected length l
 * @param s pointer to string buffer
 * @param l expected length of string (0-65535 bytes)
 * @return TRUE on all bytes read.
 */
byte dwread(byte *s, int l);

/**
 * @brief Write string at s to DriveWire with length l
 * @param s pointer to string buffer
 * @param l length of string (0-65535 bytes)
 * @return address of last char written.
 */
byte *dwwrite(byte *s, int l);

#endif /* DW_H */

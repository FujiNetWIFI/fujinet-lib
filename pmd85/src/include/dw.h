#ifndef DW_H
#define DW_H

#include <stdint.h>

/**
 * @brief Drivewire Opcodes we added for FujiNet.
 */
#define OP_FUJI 0xE2
#define OP_NET  0xE3

/**
 * @brief Read string to s from DriveWire with expected length l
 * @param s pointer to string buffer
 * @param l expected length of string (0-65535 bytes)
 * @return 1 = read successful, 0 = not successful
 */
extern uint8_t __CALLEE__  dwread(uint8_t *buf, uint16_t count);

/**
 * @brief Write string at s to DriveWire with length l
 * @param s pointer to string buffer
 * @param l length of string (0-65535 bytes)
 * @return error code
 */
extern uint8_t __CALLEE__  dwwrite(const uint8_t *buf, uint16_t count);

#endif /* DW_H */

#include <cmoc.h>
#include <coco.h>

/**
 * @brief Write string at s to DriveWire with length l
 * @param s pointer to string buffer
 * @param l length of string (0-65535 bytes)
 * @return address of last byte written
 */
byte *dwwrite(byte *s, int l)
{
  asm
    {
        pshs x,y
        ldx :s
        ldy :l
        jsr [0xD941]
        tfr x,d
        puls y,x
    }
}

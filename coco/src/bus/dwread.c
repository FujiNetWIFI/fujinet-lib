/**
 * @brief Read string to s from DriveWire with expected length l
 * @param s pointer to string buffer
 * @param l expected length of string (0-65535 bytes)
 * @return 1 = read successful, 0 = not successful
 */

#include <cmoc.h>

typedef unsigned char byte;

byte dwread(byte *s, int l)
{
    asm
    {
        pshs x,y
            ldx :s
            ldy :l
            jsr [0xD93F]
            puls y,x
            tfr cc,b
            lsrb
            lsrb
            andb #$01
            }
}

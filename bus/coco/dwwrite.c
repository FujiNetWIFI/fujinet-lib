/**
 * @brief Write string at s to DriveWire with length l
 * @param s pointer to string buffer
 * @param l length of string (0-65535 bytes)
 * @return error code
 */

#include <cmoc.h>
#include <coco.h>
#include "dw.h"

byte dwwrite(byte *s, int l)
{
    asm
    {
        pshs x,y
        ldx :s
        ldy :l
#ifdef DRAGON
        jsr [0xFA00]
#else        
        jsr [0xD941]
#endif
        tfr cc,d
        puls y,x
    }
}

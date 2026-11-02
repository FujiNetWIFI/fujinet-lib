#include <stdbool.h>

/*
 * Reads the BIOS byte at 0x002B via RDSLT, since page 0 is the DOS kernel
 * under MSX-DOS. Bit 7 of that byte: 0 = 60Hz (NTSC), 1 = 50Hz (PAL).
 * Returns true for 60Hz.
 */
bool fuji_is_ntsc_msx(void) __naked
{
__asm
    ld   a,(0xFCC1)      ; EXPTBL: main BIOS slot
    ld   hl,0x002B       ; address to read
    call 0x000C          ; RDSLT -> A = byte, interrupts left disabled
    ei                   ; DOS runs with interrupts enabled
    rlca                 ; bit 7 -> carry
    ccf                  ; carry = !bit7 (1 when 60Hz)
    ld   a,0             ; (LD doesn't touch flags)
    rla                  ; A = carry
    ld   l,a             ; also in L for sdcccall(0)
    ret
__endasm;
}

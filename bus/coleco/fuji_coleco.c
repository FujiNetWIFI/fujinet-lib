#include "fujinet-bus-coleco.h"
#include "fujinet-coleco.h"

#include <string.h>

bool fuji_coleco_present(void)
{
  return FN_MAGIC0 == 'F' && FN_MAGIC1 == 'N' && FN_PROTOVER == FN_PROTO_VER;
}

uint8_t fuji_coleco_boot_state(void)
{
  return FN_BOOTSTAT;
}

uint8_t fuji_coleco_boot_percent(void)
{
  return FN_BOOTPCT;
}

uint8_t fuji_coleco_boot_error(void)
{
  return FN_BOOTERR;
}

/*
  The swap stub, and why it is shaped like this.

  The cartridge replaces every byte of $8000-$FFFF between one read and the
  next, so whatever triggers it must not itself live in cartridge space. It
  goes in our own BSS, which is in the console's RAM.

  The vblank interrupt is on the Z80's /NMI and cannot be masked with DI, and
  the BIOS vectors it straight to $8021 -- a byte that belongs to the NEXT
  image the moment we swap. So the stub silences the VDP first, clears the flag
  that is already latched, lets any in-flight NMI be taken while our own
  handler is still there, and only then swaps.

  It also resets the VDP registers before jumping. The BIOS's $55AA path is
  LD HL,($800A) / JP (HL) with no VDP or RAM initialisation at all, so a game
  that assumes a cold VDP would otherwise inherit whatever mode we left behind.
  Silencing the sound chip is the same courtesy.
*/
static uint8_t stub[64];

static const uint8_t stub_src[] = {
  /* Screen off, VDP interrupt off: write $00 to VDP register 1. */
  0x3E, 0x00,             /* ld  a,$00              */
  0xD3, 0xBF,             /* out ($bf),a            */
  0x3E, 0x81,             /* ld  a,$81              ; register 1 */
  0xD3, 0xBF,             /* out ($bf),a            */
  /* Clear the VDP's latched interrupt flag (and de-assert /NMI). */
  0xDB, 0xBF,             /* in  a,($bf)            */
  /* Give any NMI already in flight a moment to be taken while OUR $8021
     handler is still the live one. */
  0x00, 0x00, 0x00, 0x00, /* nop x4                 */
  0x00, 0x00, 0x00, 0x00,
  /* Silence the SN76489: attenuate all four channels. */
  0x3E, 0x9F, 0xD3, 0xFF, /* ld a,$9f / out ($ff),a */
  0x3E, 0xBF, 0xD3, 0xFF,
  0x3E, 0xDF, 0xD3, 0xFF,
  0x3E, 0xFF, 0xD3, 0xFF,
  /* THE SWAP: one read of $FDFE. Every byte of cartridge space changes
     between this instruction and the next. */
  0x3A, 0xFE, 0xFD,       /* ld  a,($fdfe)          */
  /* Registers 0..7 to a power-on-like state, so a $55AA game that skips the
     BIOS title screen does not inherit our video mode. */
  0x21, 0x00, 0x80,       /* ld  hl,$8000           ; l=data, h=$80|reg */
  0x06, 0x08,             /* ld  b,8                */
  0x7D,                   /* vloop: ld a,l          */
  0xD3, 0xBF,             /* out ($bf),a            ; data byte (0) */
  0x7C,                   /* ld  a,h                */
  0xD3, 0xBF,             /* out ($bf),a            ; $80|reg */
  0x24,                   /* inc h                  */
  0x10, 0xF8,             /* djnz vloop             */
  /* Cold start: the BIOS re-reads the new image's $8000 header, so $55AA and
     $AA55 behave exactly as they would on a real cartridge. */
  0xC3, 0x00, 0x00        /* jp  $0000              */
};

void fuji_coleco_boot_swap(void)
{
  memcpy(stub, stub_src, sizeof stub_src);
  fn_regwr(FNR_BOOTLOCK, FN_BOOTLOCK_MAGIC);
  ((void (*)(void)) stub)();
  for (;;)
    ;                           /* not reached */
}

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

/* Runs from console RAM: the swap replaces all of $8000-$FFFF. It silences the VDP and NMI first, since the NMI vectors
   into the next image, and resets the VDP registers because the BIOS $55AA path does not. */
static uint8_t stub[64];

static const uint8_t stub_src[] = {
  /* Screen off, VDP interrupt off: write $00 to VDP register 1. */
  0x3E, 0x00,             /* ld  a,$00              */
  0xD3, 0xBF,             /* out ($bf),a            */
  0x3E, 0x81,             /* ld  a,$81              ; register 1 */
  0xD3, 0xBF,             /* out ($bf),a            */
  /* Clear the VDP's latched interrupt flag (and de-assert /NMI). */
  0xDB, 0xBF,             /* in  a,($bf)            */
  /* Let an in-flight NMI be taken while our handler is still live. */
  0x00, 0x00, 0x00, 0x00, /* nop x4                 */
  0x00, 0x00, 0x00, 0x00,
  /* Silence the SN76489: attenuate all four channels. */
  0x3E, 0x9F, 0xD3, 0xFF, /* ld a,$9f / out ($ff),a */
  0x3E, 0xBF, 0xD3, 0xFF,
  0x3E, 0xDF, 0xD3, 0xFF,
  0x3E, 0xFF, 0xD3, 0xFF,
  /* The swap: one read of $FDFE. */
  0x3A, 0xFE, 0xFD,       /* ld  a,($fdfe)          */
  /* VDP registers 0..7 to power-on state. */
  0x21, 0x00, 0x80,       /* ld  hl,$8000           ; l=data, h=$80|reg */
  0x06, 0x08,             /* ld  b,8                */
  0x7D,                   /* vloop: ld a,l          */
  0xD3, 0xBF,             /* out ($bf),a            ; data byte (0) */
  0x7C,                   /* ld  a,h                */
  0xD3, 0xBF,             /* out ($bf),a            ; $80|reg */
  0x24,                   /* inc h                  */
  0x10, 0xF7,             /* djnz vloop             */
  /* Cold start; the BIOS re-reads the new image header. */
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

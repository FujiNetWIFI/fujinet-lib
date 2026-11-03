#ifndef FUJINET_COLECO_H
#define FUJINET_COLECO_H

/*
  ColecoVision-only facilities that the cross-platform API has no place for.

  One rule this header cannot enforce but every caller must keep: the vblank
  interrupt arrives on the Z80's /NMI, DI will not stop it, and it therefore
  lands in the middle of transactions by design. That is harmless as long as
  the handler never reads $F800 or above -- a register stays armed across an
  interruption and the TX stream is append-only, so an interrupted transaction
  simply resumes. A handler that touches the mailbox pages will corrupt
  whatever transaction it interrupted. The other half of the rule: no code or
  data of yours may land in $F800-$FFFF (the 32K image contract, MekkoGX's).
*/

#include <fujinet-int.h>

/* fuji_coleco_boot_state() values. */
#define FUJI_COLECO_BOOT_IDLE   0
#define FUJI_COLECO_BOOT_XFER   1
#define FUJI_COLECO_BOOT_READY  2
#define FUJI_COLECO_BOOT_FAILED 0x80

/*
  Is a FujiNet cartridge actually underneath us, and does it speak a protocol
  version we understand? On a plain game cartridge these addresses hold
  whatever the game put there, or 0xFF.
*/
extern bool fuji_coleco_present(void);

/*
  fuji_mount_image() only starts the transfer. The image itself arrives
  asynchronously, pushed to the cartridge while the console keeps running, so
  poll fuji_coleco_boot_state() until it reads READY (or FAILED, in which case
  fuji_coleco_boot_error() says why). fuji_coleco_boot_percent() runs 0-100 and
  is there to drive a progress bar.
*/
extern uint8_t fuji_coleco_boot_state(void);
extern uint8_t fuji_coleco_boot_percent(void);
extern uint8_t fuji_coleco_boot_error(void);

/*
  Hand the console the image that was just pushed, and cold start it. Every
  byte of $8000-$FFFF changes between one instruction and the next, so this
  runs a stub copied into RAM; it blanks the VDP, lets any in-flight NMI be
  taken while our own handler is still live, silences the sound chip, takes the
  swap and jumps to the BIOS cold start. Does not return.

  Only meaningful once fuji_coleco_boot_state() reads READY.
*/
extern void fuji_coleco_boot_swap(void);

#endif /* FUJINET_COLECO_H */

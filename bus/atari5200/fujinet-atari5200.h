#ifndef FUJINET_ATARI5200_H
#define FUJINET_ATARI5200_H

/*
  Atari 5200 facilities the cross-platform API has no place for.

  A FujiNet client is a 32K image at $4000-$BFFF built through MekkoGX's
  atari5200.mk: cc65's own crt0 starts it through the BIOS, the linker
  config puts ROM around the cart's mailbox at $B000-$B7FF (the console's 16K
  of RAM holds DATA, BSS, the heap and the C stack), and
  atari5200-romstamp.py writes the "FUJI" claim at $BFE0 and checks the
  mailbox's rules. None of that is in the lib; the lib is the bus.

  The 5200's cart edge has no R/W line: never store into $4000-$BFFF. The
  cart drives the bus on every access there, as a ROM would.
*/

#include <fujinet-int.h>

/* fuji_a5200_boot_state() values. */
#define FUJI_A5200_BOOT_IDLE   0
#define FUJI_A5200_BOOT_XFER   1
#define FUJI_A5200_BOOT_READY  2
#define FUJI_A5200_BOOT_FAILED 0x80

/* fuji_a5200_mode() values: what the cart is serving. */
#define FUJI_A5200_MODE_BOOT 0
#define FUJI_A5200_MODE_GAME 2
#define FUJI_A5200_MODE_APP  3

/* fuji_a5200_staged() bits: the image the last mount pushed. */
#define FUJI_A5200_STAGED_READY  0x01
#define FUJI_A5200_STAGED_CLAIM  0x02   /* a FujiNet app */
#define FUJI_A5200_STAGED_CONFIG 0x04   /* CONFIG, staged to go back to */

/* Seconds before fn_commit() gives up (at least the cart's own budget for
   that call), and the last failed call's reason (the cart's error, 0xFF if
   it never answered, 144 on a NAK), named as in fujinet-lib's SIO targets. */
extern uint8_t fn_default_timeout;
extern uint8_t fn_device_error;

/* Is a FujiNet cartridge underneath us, speaking a protocol we know? */
extern bool fuji_a5200_present(void);

/*
  fuji_mount_disk_image() makes the FujiNet push the image to the cart; the
  push may finish before or after the call returns, so wait for the state
  to read READY (or FAILED, with fuji_a5200_boot_error() saying why) before
  booting. While it runs, percent runs 0-100, and got and total are byte
  counts (total is 0 until the stream opens).
*/
extern uint8_t fuji_a5200_boot_state(void);
extern uint8_t fuji_a5200_boot_percent(void);
extern uint8_t fuji_a5200_boot_error(void);
extern uint32_t fuji_a5200_boot_got(void);
extern uint32_t fuji_a5200_boot_total(void);

extern uint8_t fuji_a5200_mode(void);
extern uint8_t fuji_a5200_staged(void);

/*
  Leave for good: interrupts and DMA off, the chips and RAM cleared as at
  power-on, then the console's own BIOS starts the next image, which the
  cart swaps in as the BIOS looks. fuji_a5200_boot() starts the staged image
  (once fuji_a5200_boot_state() reads READY); fuji_a5200_config() goes back
  to CONFIG. Either returns false only if the cart never armed the swap.
*/
extern bool fuji_a5200_boot(void);
extern bool fuji_a5200_config(void);

#endif /* FUJINET_ATARI5200_H */

#ifndef FUJINET_ATARI5200_H
#define FUJINET_ATARI5200_H

/*
  Atari 5200 facilities the cross-platform API has no place for.

  A FujiNet client is a 32K image at $4000-$BFFF built through
  makefiles/platforms/atari5200.mk: its crt0, its linker config (ROM around
  the cart's mailbox at $B000-$B7FF; the console's 16K of RAM holds DATA,
  BSS, the heap and the C stack) and atari5200-romstamp.py, which checks the
  "FUJI" claim at $BFE0 and the mailbox's rules.

  The 5200's cart edge has no R/W line: never store into $4000-$BFFF. The
  cart drives the bus on every access there, as a ROM would.

  The crt0 does what the BIOS does at power-on (its header skips the BIOS's
  title) and keeps the BIOS's zero page and vectors, so cc65's atari5200
  library works; its VBI counts frames in OS.rtclok. The keypad's IRQ is on
  and fuji_a5200_key holds the last key.
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

/* fuji_a5200_key: the last keypad key + 1, 0 = none since the program
   cleared it. */
#define FUJI_A5200_KEY_STAR   (0x0A + 1)
#define FUJI_A5200_KEY_HASH   (0x0B + 1)
#define FUJI_A5200_KEY_START  (0x0C + 1)
#define FUJI_A5200_KEY_PAUSE  (0x0D + 1)
#define FUJI_A5200_KEY_RESET  (0x0E + 1)
#define FUJI_A5200_KEY_DIGIT(d) ((d) + 1)
extern volatile uint8_t fuji_a5200_key;

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

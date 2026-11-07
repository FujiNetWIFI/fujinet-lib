#ifndef FUJINET_COLECO_H
#define FUJINET_COLECO_H

/* The vblank NMI cannot be masked, so a handler must never read $F800 or above, and no code or data
   may land in $F800-$FFFF (the 32K image contract, MekkoGX's). */

#include <fujinet-int.h>

/* fuji_coleco_boot_state() values. */
enum FujiColecoBoot
{
  FUJI_COLECO_BOOT_IDLE   = 0,
  FUJI_COLECO_BOOT_XFER   = 1,
  FUJI_COLECO_BOOT_READY  = 2,
  FUJI_COLECO_BOOT_FAILED = 0x80
};

/* True if a FujiNet cartridge with a protocol version we speak is present. */
extern bool fuji_coleco_present(void);

/* fuji_mount_disk_image() only starts the transfer: poll boot_state until READY or FAILED (boot_error says why).
   boot_percent runs 0-100. */
extern uint8_t fuji_coleco_boot_state(void);
extern uint8_t fuji_coleco_boot_percent(void);
extern uint8_t fuji_coleco_boot_error(void);

/* Swap in the pushed image and cold start it; does not return. Only valid once boot_state is READY. */
extern void fuji_coleco_boot_swap(void);

#endif /* FUJINET_COLECO_H */

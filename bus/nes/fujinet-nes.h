#ifndef FUJINET_NES_H
#define FUJINET_NES_H

/* NES-only facilities. Build through makefiles/platforms/nes.mk: its linker config keeps $FFF0-$FFF9 clear
   for the "FUJI" claim that nes-romstamp.py writes, without which the cart shuts the mailbox down at boot. */

#include <fujinet-int.h>

/* fuji_nes_boot_state() values. */
enum FujiNesBoot {
  FUJI_NES_BOOT_IDLE   = 0,
  FUJI_NES_BOOT_XFER   = 1,
  FUJI_NES_BOOT_READY  = 2,
  FUJI_NES_BOOT_FAILED = 0x80
};

/* True if a FujiNet cartridge with a protocol version we speak is present; elsewhere these addresses are open bus. */
extern bool fuji_nes_present(void);

/* fuji_mount_disk_image() blocks until the whole image is on the cart. For progress, start MOUNT_IMAGE yourself,
   set FNR_SEQ, and poll these until FN_ACKSEQ echoes it; boot_state then reads READY or FAILED (boot_error says why).
   boot_percent runs 0-100; boot_got/boot_total are byte counts, total 0 until the stream opens. */
extern uint8_t fuji_nes_boot_state(void);
extern uint8_t fuji_nes_boot_percent(void);
extern uint8_t fuji_nes_boot_error(void);
extern uint32_t fuji_nes_boot_got(void);
extern uint32_t fuji_nes_boot_total(void);

/* Arm the load and jump to the cart's loader. Does not return; call once boot_state reads READY. */
extern void fuji_nes_boot(void);

#endif /* FUJINET_NES_H */

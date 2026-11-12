#ifndef FUJINET_BUS_NES_H
#define FUJINET_BUS_NES_H

/* Mirrors fujinet-firmware/pico/nes/firmware/include/fuji_mailbox.h; addresses are console addresses.
   Console -> cart is a write to a hotspot page, cart -> console is memory the cart paints.
   Mailbox stores at $5500-$57FF must be plain STA/STX/STY: a read-modify-write (INC, ASL, `++`, `|=`)
   writes the old value back first and lands as a spurious event. nes-romstamp.py rejects those opcodes. */

#include <fujinet-bus.h>

/* ---- cart -> console: painted memory ---- */

/* The whole reply is read in place from cartridge memory. */
#define FN_REPLY     ((volatile uint8_t *) 0x5000)
#define FN_REPLY_MAX FUJI_NETWORK_READ_MAX

#define FN_ACKSEQ    (*(volatile uint8_t *) 0x5400)
#define FN_STATUS    (*(volatile uint8_t *) 0x5401)
#define FN_ERRCODE   (*(volatile uint8_t *) 0x5402)
#define FN_REPLYCMD  (*(volatile uint8_t *) 0x5403)
#define FN_RXLEN_LO  (*(volatile uint8_t *) 0x5404)
#define FN_RXLEN_HI  (*(volatile uint8_t *) 0x5405)
#define FN_BOOTSTAT  (*(volatile uint8_t *) 0x5406)
#define FN_BOOTPCT   (*(volatile uint8_t *) 0x5407)
#define FN_BOOTERR   (*(volatile uint8_t *) 0x5408)
#define FN_MAGIC0    (*(volatile uint8_t *) 0x5409)
#define FN_MAGIC1    (*(volatile uint8_t *) 0x540A)
#define FN_PROTOVER  (*(volatile uint8_t *) 0x540B)
#define FN_LINK      (*(volatile uint8_t *) 0x5415)
#define FN_BOOTGOT   ((volatile uint8_t *) 0x5416)   /* 24-bit LE */
#define FN_BOOTTOT   ((volatile uint8_t *) 0x5419)   /* 24-bit LE */

#define FN_STATUS_LINK 0x01
#define FN_STATUS_BUSY 0x02

#define FN_PROTO_VER   1

/* ---- console -> cart: write-only hotspot pages ---- */
#define FN_REGSEL    ((volatile uint8_t *) 0x5500)   /* [reg] = value */
#define FN_TXPAGE    (*(volatile uint8_t *) 0x5700)  /* = byte: append */
#define FN_LOADER    0x5800                          /* the swap stub */

enum FujiNesReg {
  FNR_DEVICE   = 0x00,
  FNR_CMD      = 0x01,
  FNR_NPARAM   = 0x02,
  FNR_DATA_RST = 0x05,
  FNR_RXSLICE  = 0x06,
  FNR_SEQ      = 0x10,
  FNR_BOOTLOCK = 0x11
};

#define FN_BOOTLOCK_MAGIC 0xB5

/* TX stream: NPARAM x { size byte (1|2|4), little-endian value }, then the raw payload.
   A network write carries one 16-bit parameter, three stream bytes. */
#define FN_TX_MAX    (FUJI_NETWORK_WRITE_MAX + 3)

/* fn_commit() results: the cart's own error codes, plus FN_EWAIT when it never answered. */
enum FujiNesErr {
  FN_OK        = 0,
  FN_ENOLINK   = 1,
  FN_ETIMEOUT  = 2,
  FN_EBADFRAME = 3,
  FN_ETOOBIG   = 4,
  FN_EWAIT     = 0xFF
};

/* Functions, not macros, so each mailbox store stays a plain `sta` whatever the optimiser does. */
extern void __fastcall__ fn_tx(uint8_t b);
extern void fn_regwr(uint8_t reg, uint8_t val);
extern uint8_t fn_commit(void);

/* Length of the reply left in the window by the last fuji_bus_call(). */
extern uint16_t fuji_bus_call_rlen;

#endif /* FUJINET_BUS_NES_H */

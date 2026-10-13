#ifndef FUJINET_BUS_ATARI5200_H
#define FUJINET_BUS_ATARI5200_H

/*
  The Atari 5200's half of the FujiNet cartridge mailbox.

  Mirrors fujinet-firmware/pico/atari-5200/firmware/include/fuji_mailbox.h by
  hand, which is the source of truth; keep the two in step. The edge has no
  R/W line, so every byte reaches the cart in the address of a READ:
  register n = v is a read of $B500+n then a read of $B600+v, and a TX byte
  b is a read of $B700+b. The reply is memory the cart paints at $B000,
  read in place. The arena is $B000-$B7FF, inside the cart's own window.

  Touch the hotspot pages ($B500-$B7FF) only through fn_tx() and fn_regwr():
  a C expression may compile to (zp),Y or a read-modify-write, and the
  cart counts every read there. atari5200-romstamp.py rejects stores into
  the cart window and off-page indexed reads of the hotspot pages.
*/

#include <fujinet-bus.h>

/* ---- cart -> console: painted memory ---- */
#define FN_REPLY     ((volatile uint8_t *) 0xB000)
#define FN_REPLY_MAX 1024

#define FN_ACKSEQ    (*(volatile uint8_t *) 0xB400)
#define FN_STATUS    (*(volatile uint8_t *) 0xB401)
#define FN_ERRCODE   (*(volatile uint8_t *) 0xB402)
#define FN_REPLYCMD  (*(volatile uint8_t *) 0xB403)
#define FN_RXLEN_LO  (*(volatile uint8_t *) 0xB404)
#define FN_RXLEN_HI  (*(volatile uint8_t *) 0xB405)
#define FN_BOOTSTAT  (*(volatile uint8_t *) 0xB406)
#define FN_BOOTPCT   (*(volatile uint8_t *) 0xB407)
#define FN_BOOTERR   (*(volatile uint8_t *) 0xB408)
#define FN_MAGIC0    (*(volatile uint8_t *) 0xB409)
#define FN_MAGIC1    (*(volatile uint8_t *) 0xB40A)
#define FN_PROTOVER  (*(volatile uint8_t *) 0xB40B)
#define FN_MODE      (*(volatile uint8_t *) 0xB412)
#define FN_MAPPER    (*(volatile uint8_t *) 0xB414)
#define FN_LINK      (*(volatile uint8_t *) 0xB415)
#define FN_BOOTGOT   ((volatile uint8_t *) 0xB416)    /* 24-bit LE */
#define FN_BOOTTOT   ((volatile uint8_t *) 0xB419)    /* 24-bit LE */
#define FN_ARMED     (*(volatile uint8_t *) 0xB41C)
#define FN_STAGED    (*(volatile uint8_t *) 0xB41D)
#define FN_STAGEDKIND (*(volatile uint8_t *) 0xB41E)
#define FN_STAGEDCRC ((volatile uint8_t *) 0xB41F)    /* 32-bit LE */
#define FN_SWAPS     (*(volatile uint8_t *) 0xB423)

#define FN_STATUS_LINK 0x01
#define FN_PROTO_VER   1

/* ---- console -> cart: reads ---- */
#define FNR_DEVICE   0x00
#define FNR_CMD      0x01
#define FNR_NPARAM   0x02
#define FNR_DATA_RST 0x05
#define FNR_RXSLICE  0x06
#define FNR_SEQ      0x10
#define FNR_BOOTLOCK 0x11
#define FNR_CONFIG   0x14

#define FN_BOOTLOCK_MAGIC 0xB5
#define FN_CONFIG_MAGIC   0xC5

/* NPARAM x { size 1|2|4, value LE }, then the payload. */
#define FN_TX_MAX    320

/* fn_commit() results: the cart's own error codes, plus our own timeout. */
#define FN_OK        0
#define FN_ENOLINK   1
#define FN_ETIMEOUT  2
#define FN_EBADFRAME 3
#define FN_ETOOBIG   4
#define FN_EWAIT     0xFF  /* the cart never answered at all */

/* One register write is two hotspot reads; one TX byte is one. Assembly
   (fuji_atari5200_go.s), so each is exactly `lda base,y`. */
extern void __fastcall__ fn_tx(uint8_t b);
extern void __fastcall__ fn_regwr(uint8_t reg, uint8_t val);
extern uint8_t fn_commit(void);

/* The last fuji_bus_call()'s reply length, clamped to its buffer when it
   copied one. */
extern uint16_t fuji_bus_call_rlen;

#endif /* FUJINET_BUS_ATARI5200_H */

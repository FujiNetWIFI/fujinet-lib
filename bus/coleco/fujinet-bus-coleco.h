#ifndef FUJINET_BUS_COLECO_H
#define FUJINET_BUS_COLECO_H

/* Mirrors fuji_mailbox.h in fujinet-firmware/pico/coleco; addresses are console addresses.
   Both directions ride cartridge reads: the address carries the payload, the reply is repainted ROM. */

#include <fujinet-bus.h>

/* ---- cart -> console: repainted ROM ---- */

/* The whole reply is read in place from cartridge ROM, saving a bounce buffer in scarce console RAM. */
#define FN_REPLY     ((volatile uint8_t *) 0xF800)
#define FN_REPLY_MAX FUJI_NETWORK_READ_MAX

#define FN_ACKSEQ    (*(volatile uint8_t *) 0xFC00)
#define FN_STATUS    (*(volatile uint8_t *) 0xFC01)
#define FN_ERRCODE   (*(volatile uint8_t *) 0xFC02)
#define FN_REPLYCMD  (*(volatile uint8_t *) 0xFC03)
#define FN_RXLEN_LO  (*(volatile uint8_t *) 0xFC04)
#define FN_RXLEN_HI  (*(volatile uint8_t *) 0xFC05)
#define FN_BOOTSTAT  (*(volatile uint8_t *) 0xFC06)
#define FN_BOOTPCT   (*(volatile uint8_t *) 0xFC07)
#define FN_BOOTERR   (*(volatile uint8_t *) 0xFC08)
#define FN_MAGIC0    (*(volatile uint8_t *) 0xFC09)
#define FN_MAGIC1    (*(volatile uint8_t *) 0xFC0A)
#define FN_PROTOVER  (*(volatile uint8_t *) 0xFC0B)

#define FN_STATUS_LINK 0x01
#define FN_STATUS_BUSY 0x02

#define FN_PROTO_VER   1

/* ---- console -> cart: hotspot reads ---- */
#define FN_REGSEL    ((volatile uint8_t *) 0xFD00)
#define FN_REGDAT    ((volatile uint8_t *) 0xFE00)
#define FN_TXPAGE    ((volatile uint8_t *) 0xFF00)
#define FN_SWAP      0xFDFE

/* Register file, reached through a REGSEL / REGDAT read pair. */
enum FnReg
{
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
enum FnCartError
{
  FN_OK        = 0,
  FN_ENOLINK   = 1,
  FN_ETIMEOUT  = 2,
  FN_EBADFRAME = 3,
  FN_ETOOBIG   = 4,
  FN_EWAIT     = 0xFF
};

/* The read must be stored to a volatile: sccz80 drops an unused load, and the read is the message. */
extern volatile uint8_t fn_sink;

#define FN_TOUCH(addr) (fn_sink = *(volatile uint8_t *)(addr))

/* Length of the reply left in the window by the last fuji_bus_call(). */
extern uint16_t fuji_bus_call_rlen;

extern void fn_regwr(uint8_t reg, uint8_t val);
extern uint8_t fn_commit(void);

#endif /* FUJINET_BUS_COLECO_H */

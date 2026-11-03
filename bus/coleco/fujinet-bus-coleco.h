#ifndef FUJINET_BUS_COLECO_H
#define FUJINET_BUS_COLECO_H

/*
  The ColecoVision's half of the FujiNet mailbox.

  These equates mirror fujinet-firmware/pico/coleco/firmware/include/fuji_mailbox.h
  by hand, which is the upstream source of truth; every address here is a
  CONSOLE address, i.e. the image offset plus $8000. Keep the two in step.

  The cartridge connector carries A0-A14, D0-D7, four pre-decoded chip selects
  and power. There is no /RD, /WR or /MREQ, so the cartridge cannot tell a read
  from a write -- which is why every real ColecoVision mapper decodes the
  address. Both directions of the mailbox therefore ride the read path:

    console -> cart   the ADDRESS carries the payload. Reading $FD05 arms
                      register 5; reading $FE2A then sets it to 0x2A. Reading
                      $FF41 appends 'A' to the outgoing stream.
    cart -> console   the cartridge repaints its own ROM, so by the time the
                      Z80 looks, the reply at $F800 is just bytes.

  There is no framing, no checksum and no ACK/NAK exchange on this side. SLIP
  and the end-around-carry checksum live between the RP2040 and the ESP32,
  where the console never sees them.
*/

#include <fujinet-bus.h>

/* ---- cart -> console: repainted ROM ---- */

/* The whole reply in one piece, not a paged 256-byte slice as on the sibling
   cartridge ports. The window is 32K here and the CONSOLE is what is scarce --
   about 700 usable bytes of RAM once the BIOS tables and the stack are paid
   for -- so a reply that can be read in place out of cartridge ROM saves a
   bounce buffer and a polling handshake on every transaction. */
#define FN_REPLY     ((volatile uint8_t *) 0xF800)
#define FN_REPLY_MAX 1024

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
#define FNR_DEVICE   0x00
#define FNR_CMD      0x01
#define FNR_NPARAM   0x02
#define FNR_DATA_RST 0x05
#define FNR_RXSLICE  0x06
#define FNR_SEQ      0x10
#define FNR_BOOTLOCK 0x11

#define FN_BOOTLOCK_MAGIC 0xB5

/* The TX stream is NPARAM x { size byte (1|2|4), then that many value bytes,
   little-endian }, followed by the raw payload. */
#define FN_TX_MAX    320

/* fn_commit() results: the cart's own error codes, plus our own timeout. */
#define FN_OK        0
#define FN_ENOLINK   1
#define FN_ETIMEOUT  2
#define FN_EBADFRAME 3
#define FN_ETOOBIG   4
#define FN_EWAIT     0xFF  /* the cart never answered at all */

/*
  Every mailbox access is a read whose value we do not want -- the address is
  the message. Writing that as `(void) FN_REGSEL[reg];` looks obviously right
  and is silently wrong: sccz80 emits the address calculation and then throws
  the load away, volatile or not. The generated register write was

      ld de,64768     ; $FD00, computed...
      ld de,65024     ; ...and immediately overwritten by $FE00

  so the REGSEL half never happened, no transaction ever launched, and the
  caller sat polling ACKSEQ forever. Storing the byte somewhere volatile is
  what makes the read a side effect the compiler has to keep. If this is ever
  rewritten, check the generated code with `zcc +coleco -O2 -a` and look for
  two loads, not one.
*/
extern volatile uint8_t fn_sink;
extern uint8_t fn_default_timeout;
extern uint8_t fn_device_error;

#define FN_TOUCH(addr) (fn_sink = *(volatile uint8_t *)(addr))

/* Length of the reply left in the window by the last fuji_bus_call(). */
extern uint16_t fuji_bus_call_rlen;

extern void fn_regwr(uint8_t reg, uint8_t val);
extern uint8_t fn_commit(void);

#endif /* FUJINET_BUS_COLECO_H */

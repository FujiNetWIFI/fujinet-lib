/* Separate from fujinet-unapi-call.c so that code defining its own
 * fuji_unapi_call() can use discovery without a duplicate symbol. */

#include "fujinet-unapi-msx.h"

/* Discovery result, kept between calls.  unapi_entry doubles as the "looked
   yet" flag: an entry point is in page 1 or 2, so never zero. */
static uint8_t unapi_slot;
static uint8_t unapi_in_ram;
static void *unapi_entry;

static uint8_t unapi_count;

/* HOKVLD bit 0 says some ROM has installed the extended BIOS hook.  Without it
   the hook at FFCAh is not even a RET on some machines (the Philips NMS 8250
   leaves it zero), and calling it slides through NOPs to a cold reboot. */
#define HOKVLD (*(volatile uint8_t *) 0xFB20)

/* How many implementations of the API named at ARG are installed.
   In: DE = 2222h, A = 0, B = 0.  Out: B = count. */
static uint8_t UNAPIGetCount(void)
{
  const char *service = FUJI_UNAPI_ID;
  volatile char *p;
  volatile char *ARG = (volatile char *) 0xF847;


  if (!(HOKVLD & 1))
    return 0;

  for (p = ARG; *service; p++, service++)
    *p = *service;
  *p = 0;

  __asm
    push ix
    push iy
    ld   de, 0x2222
    xor  a
    ld   b, a
    call 0xFFCA         ; EXTBIO
    ld   a, b
    ld   (_unapi_count), a
    pop  iy
    pop  ix
    __endasm;

  return unapi_count;
}

/* Where the first of them is - any implementation of this API will do.
   In: DE = 2222h, A = 1, B = 1.
   Out: A = slot, B = segment (FFh if not in a mapped RAM segment),
        HL = entry point. */
static void UNAPIGetSlot(void)
{
  __asm
    push ix
    push iy
    ld   de, 0x2222
    ld   a, 1           ; implementation index
    ld   b, 1           ; function 1: where is it
    call 0xFFCA         ; EXTBIO
    ld   (_unapi_slot), a
    ld   (_unapi_entry), hl
    ld   a, b
    ld   (_unapi_in_ram), a
    pop  iy
    pop  ix
    __endasm;
}

static uint8_t scan_slot;
static uint16_t scan_addr;
static uint8_t scan_byte;
static uint16_t init_addr;
static uint8_t scan_tried;

static uint8_t read_slot(uint8_t slot, uint16_t addr)
{
  scan_slot = slot;
  scan_addr = addr;
  __asm
    push ix
    push iy
    ld   a, (_scan_slot)
    ld   hl, (_scan_addr)
    call 0x000C         ; RDSLT
    ld   (_scan_byte), a
    pop  iy
    pop  ix
    __endasm;
  return scan_byte;
}

static void call_init(uint8_t slot, uint16_t addr)
{
  scan_slot = slot;
  init_addr = addr;
  __asm
    push ix
    push iy
    ld   ix, (_init_addr)
    ld   a, (_scan_slot)
    push af
    pop  iy             ; IYh = slot
    call 0x001C         ; CALSLT
    pop  iy
    pop  ix
    __endasm;
}

static uint8_t slot_has_rom(uint8_t slot)
{
  const char *id = FUJI_UNAPI_ID;
  uint8_t i;

  for (i = 0; i < sizeof(FUJI_UNAPI_ID); i++)
    if (read_slot(slot, FUJI_UNAPI_ROM_ID_ADDR + i) != (uint8_t) id[i])
      return 0;
  return 1;
}

/* A user ROM in a lower slot runs first and may never return, so the BIOS
   never reaches UNAPI's INIT and EXTBIO stays unhooked. */
static uint8_t install_from_rom(void)
{
  uint8_t p, s, slot;

  for (p = 0; p < 4; p++) {
    uint8_t subslots = (*(volatile uint8_t *) (0xFCC1 + p)) & 0x80 ? 4 : 1;

    for (s = 0; s < subslots; s++) {
      slot = subslots > 1 ? (0x80 | (s << 2) | p) : p;
      if (slot_has_rom(slot)) {
        call_init(slot, read_slot(slot, 0x4002) | (read_slot(slot, 0x4003) << 8));
        return 1;
      }
    }
  }
  return 0;
}

static uint8_t discover(void)
{
  if (!unapi_entry) {
    if (!UNAPIGetCount()) {
      if (scan_tried)
        return 0;
      scan_tried = 1;
      if (!install_from_rom() || !UNAPIGetCount())
        return 0;
    }
    UNAPIGetSlot();
#ifdef DEBUG
    printf("IN SLOT %d RAM: %d ENTRY: 0x%04x\n", unapi_slot, unapi_in_ram, unapi_entry);
#endif
    if (!unapi_entry)
      return 0;
  }
  return 1;
}

uint8_t fuji_unapi_slot(void)
{
  return discover() ? unapi_slot : 0xFF;
}

void *fuji_unapi_entry(void)
{
  return discover() ? unapi_entry : 0;
}

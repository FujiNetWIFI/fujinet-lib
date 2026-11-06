#include "fujinet-fuji.h"
#include "fujinet-unapi-msx.h"
#include <string.h>

#define STUB_ADDR       0xC000
#define STUB_SLOT       1
#define STUB_MODE       12
#define STUB_NO_SLOT    8

/* Runs from RAM because the write swaps out the ROM the caller may run from.
 * The IO window is only decoded with the UNAPI subslot in page 2. */
static const uint8_t cart_mode_stub[] = {
  0x3E, 0x00,                    /* LD A,slot */
  0x21, 0x00, 0x80,              /* LD HL,8000h */
  0xCD, 0x24, 0x00,              /* CALL ENASLT */
  0x21, 0xFF, 0xBF,              /* LD HL,BFFFh */
  0x36, 0x00,                    /* LD (HL),mode */
  0xC3, 0x00, 0x00               /* JP 0 */
};

void fuji_set_cart_mode(uint8_t mode)
{
  uint8_t slot = fuji_unapi_slot();
  volatile uint8_t *stub = (volatile uint8_t *) STUB_ADDR;
  void (*entry)(void) = (void (*)(void)) STUB_ADDR;

  memcpy((void *) STUB_ADDR, cart_mode_stub, sizeof(cart_mode_stub));
  stub[STUB_SLOT] = slot;
  stub[STUB_MODE] = mode;
  if (slot == 0xFF)
    entry = (void (*)(void)) (STUB_ADDR + STUB_NO_SLOT);

  entry();
}

void fuji_enable_user_rom(void)
{
  fuji_set_cart_mode(FUJI_CART_MODE_UNAPI_USER_ROM);
}

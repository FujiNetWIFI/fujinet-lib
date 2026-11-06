#include "fujinet-bus-nes.h"
#include "fujinet-nes.h"

bool fuji_nes_present(void)
{
  return FN_MAGIC0 == 'F' && FN_MAGIC1 == 'N' && FN_PROTOVER == FN_PROTO_VER;
}

uint8_t fuji_nes_boot_state(void)
{
  return FN_BOOTSTAT;
}

uint8_t fuji_nes_boot_percent(void)
{
  return FN_BOOTPCT;
}

uint8_t fuji_nes_boot_error(void)
{
  return FN_BOOTERR;
}

/* The cart updates these a byte at a time, so re-read until two reads match. */
static uint32_t count24(volatile uint8_t *p)
{
  uint32_t a, b;

  b = (uint32_t) p[0] | ((uint32_t) p[1] << 8) | ((uint32_t) p[2] << 16);
  do {
    a = b;
    b = (uint32_t) p[0] | ((uint32_t) p[1] << 8) | ((uint32_t) p[2] << 16);
  } while (a != b);

  return a;
}

uint32_t fuji_nes_boot_got(void)
{
  return count24(FN_BOOTGOT);
}

uint32_t fuji_nes_boot_total(void)
{
  return count24(FN_BOOTTOT);
}

/* Jump to the loader ROM at $5800, which copies the staged image into the SRAMs and cold-starts it. */
void fuji_nes_boot(void)
{
  fn_regwr(FNR_BOOTLOCK, FN_BOOTLOCK_MAGIC);
  __asm__("sei");
  *(volatile uint8_t *) 0x2000 = 0;     /* NMI off */
  *(volatile uint8_t *) 0x2001 = 0;     /* rendering off */
  *(volatile uint8_t *) 0x4015 = 0;     /* APU channels off */
  ((void (*)(void)) FN_LOADER)();
  for (;;)
    ;
}

#include "fujinet-bus-atari5200.h"
#include "fujinet-atari5200.h"

bool fuji_a5200_present(void)
{
  return FN_MAGIC0 == 'F' && FN_MAGIC1 == 'N' && FN_PROTOVER == FN_PROTO_VER;
}

uint8_t fuji_a5200_boot_state(void)
{
  return FN_BOOTSTAT;
}

uint8_t fuji_a5200_boot_percent(void)
{
  return FN_BOOTPCT;
}

uint8_t fuji_a5200_boot_error(void)
{
  return FN_BOOTERR;
}

/* The cart writes these a byte at a time during the push; two matching
   reads in a row cannot catch a carry half-propagated. */
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

uint32_t fuji_a5200_boot_got(void)
{
  return count24(FN_BOOTGOT);
}

uint32_t fuji_a5200_boot_total(void)
{
  return count24(FN_BOOTTOT);
}

uint8_t fuji_a5200_mode(void)
{
  return FN_MODE;
}

uint8_t fuji_a5200_staged(void)
{
  return FN_STAGED;
}

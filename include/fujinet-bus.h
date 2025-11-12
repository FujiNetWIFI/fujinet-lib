#ifndef FUJINET_BUS_H
#define FUJINET_BUS_H

// Convenience macros to fill in FujiDCB and make the call
#include <fujinet-bus-ezcall.h>
#include <fujinet-int.h>

#define FUJI_DEVICEID_FUJINET           0x70

#define FUJI_DEVICEID_CLOCK             0x45
#define FUJI_DEVICEID_CPM               0x5A
#define FUJI_DEVICEID_DISK              0x31
#define FUJI_DEVICEID_DISK_LAST         0x3F
#define FUJI_DEVICEID_MIDI              0x99
#define FUJI_DEVICEID_NETWORK		0x71
#define FUJI_DEVICEID_NETWORK_LAST	0x78
#define FUJI_DEVICEID_PRINTER           0x40
#define FUJI_DEVICEID_PRINTER_LAST      0x43
#define FUJI_DEVICEID_SERIAL            0x50
#define FUJI_DEVICEID_SERIAL_LAST       0x53
#define FUJI_DEVICEID_VOICE		0x43

enum {
  FUJI_FIELD_NONE        = 0,
  FUJI_FIELD_A1          = 1,
  FUJI_FIELD_A1_A2       = 2,
  FUJI_FIELD_A1_A2_A3    = 3,
  FUJI_FIELD_A1_A2_A3_A4 = 4,
  FUJI_FIELD_B12         = 5,
  FUJI_FIELD_B12_B34     = 6,
  FUJI_FIELD_C1234       = 7,
};

#define FUJI_FIELD_LOOKUP_TABLE 1
#ifdef FUJI_FIELD_LOOKUP_TABLE

// Not worth making these into functions, I'm sure they'd eat more bytes
extern uint8_t fuji_field_numbytes_table[];  // 0, 1, 2, 3, 4, 2, 4, 4
extern uint8_t fuji_field_numfields_table[]; // 0, 1, 2, 3, 4, 1, 2, 1
#define fuji_field_numbytes(descr) fuji_field_numbytes_table[descr]
#define fuji_field_numfields(descr) fuji_field_numfields_table[descr]

#else /* ! FUJI_FIELD_LOOKUP_TABLE */

#define FUJI_FIELD_16_OR_32_MASK 0x04
#define FUJI_FIELD_32_MASK       0x02

static inline uint8_t fuji_field_numbytes(uint8_t descr)
{
  if (!descr)
    return 0;
  descr--;
  if (!(descr & FUJI_FIELD_16_OR_32_MASK))
    return descr + 1;
  if (descr & FUJI_FIELD_32_MASK)
    return 4;
  return ((descr & 1) + 1) << 1; // Shift by 1 instead of multiply by 2
}

static inline uint8_t fuji_field_numfields(uint8_t descr)
{
  if (!descr)
    return 0;
  descr--;
  if (!(descr & FUJI_FIELD_16_OR_32_MASK))
    return descr + 1;
  return (descr & 1) + 1;
}

#endif /* FUJI_FIELD_LOOKUP_TABLE */

extern bool fuji_bus_call(uint8_t device, uint8_t fuji_cmd, uint8_t fields,
			  uint8_t aux1, uint8_t aux2, uint8_t aux3, uint8_t aux4,
			  const void *data, size_t data_length,
			  void *reply, size_t reply_length);
extern size_t fuji_bus_read(uint8_t device, void *buffer, size_t length);
extern size_t fuji_bus_write(uint8_t device, const void *buffer, size_t length);

extern bool fuji_bus_appkey_read(void *string, uint16_t *length);
extern bool fuji_bus_appkey_write(void *string, uint16_t length);

#endif /* FUJINET_BUS_H */

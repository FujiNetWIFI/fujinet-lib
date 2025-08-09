#ifndef FUJINET_ENDIAN_H
#define FUJINET_ENDIAN_H

#ifndef _CMOC_VERSION_
#define U32_MSW(v) (((uint32_t)(v) >> 16) & 0xFFFF)  // Most Significant Word
#define U32_LSW(v) ((uint32_t)(v) & 0xFFFF)          // Least Significant Word

#define U16_MSB(w) (((uint16_t)(w) >> 8) & 0xFF)
#define U16_LSB(w) ((uint16_t)(w) & 0xFF)
#else /* ! _CMOC_VERSION_ */
#define U32_MSW(v) ((uint32_t)(v) & 0xFFFF)          // Least Significant Word
#define U32_LSW(v) (((uint32_t)(v) >> 16) & 0xFFFF)  // Most Significant Word

#define U16_MSB(w) ((uint16_t)(w) & 0xFF)
#define U16_LSB(w) (((uint16_t)(w) >> 8) & 0xFF)
#endif /* _CMOC_VERSION_ */

#endif /* FUJINET_ENDIAN_H */

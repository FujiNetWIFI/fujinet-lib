#ifndef FUJINET_ENDIAN_H
#define FUJINET_ENDIAN_H

#if !defined(_CMOC_VERSION_) || _CMOC_VERSION_ >= 1096
#define U32_MSW(v) ((uint16_t)(((uint32_t)(v) >> 16) & 0xFFFF))  // Most Significant Word
#define U32_LSW(v) ((uint16_t)((uint32_t)(v) & 0xFFFF))          // Least Significant Word

#define U16_MSB(w) ((uint8_t)(((uint16_t)(w) >> 8) & 0xFF))
#define U16_LSB(w) ((uint8_t)((uint16_t)(w) & 0xFF))
#else /* _CMOC_VERSION_ < 1096 */
#define U32_MSW(v) ((uint16_t)((uint32_t)(v) & 0xFFFF))          // Least Significant Word
#define U32_LSW(v) ((uint16_t)(((uint32_t)(v) >> 16) & 0xFFFF))  // Most Significant Word

#define U16_MSB(w) ((uint8_t)((uint16_t)(w) & 0xFF))
#define U16_LSB(w) ((uint8_t)(((uint16_t)(w) >> 8) & 0xFF))
#endif /* _CMOC_VERSION_ */

#ifdef _CMOC_VERSION_
#define NATIVE_SPLIT_U16(w) U16_MSB(w), U16_LSB(w)
#define NATIVE_SPLIT_32(l)                                              \
  U16_MSB(U32_MSW(l)), U16_LSB(U32_MSW(l)),                             \
    U16_MSB(U32_LSW(l)), U16_LSB(U32_LSW(l))
#else
#define NATIVE_SPLIT_U16(w) U16_LSB(w), U16_MSB(w)
#define NATIVE_SPLIT_32(l)                                              \
  U16_LSB(U32_LSW(l)), U16_MSB(U32_LSW(l)),                             \
    U16_LSB(U32_MSW(l)), U16_MSB(U32_MSW(l))
#endif /* _CMOC_VERSION_ */

#endif /* FUJINET_ENDIAN_H */

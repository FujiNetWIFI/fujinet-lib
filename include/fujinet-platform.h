#ifndef FUJINET_PLATFORM_H
#define FUJINET_PLATFORM_H

#if defined(_CMOC_VERSION_)
#define FUJI_PLATFORM_COCO
#elif defined(__APPLE2__)
#define FUJI_PLATFORM_APPLE2
#elif defined(__ATARI__)
#define FUJI_PLATFORM_ATARI
#elif defined(__C64__)
#define FUJI_PLATFORM_C64
#elif defined(__MSX__)
#define FUJI_PLATFORM_MSX
#elif defined(__LYNX__)
#define FUJI_PLATFORM_LYNX
#elif defined(__DOS__) || defined(MSDOS)
#define FUJI_PLATFORM_MSDOS
#elif defined(__ADAM__) || defined(__COLECOADAM__)
#define FUJI_PLATFORM_ADAM
#elif defined(__TRS80M2__)
#define FUJI_PLATFORM_MODEL2
#else
#error "Building on unknown platform"
#endif

#endif /* FUJINET_PLATFORM_H */

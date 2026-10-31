#ifndef FUJINET_CONST_H
#define FUJINET_CONST_H

#include <fujinet-platform.h>

#define FUJI_MAX_HOSTS 8
#if defined(FUJI_PLATFORM_COCO) || defined(FUJI_PLATFORM_PMD85) \
  || defined(FUJI_PLATFORM_ADAM)
#define FUJI_MAX_DISKS 4
#elif defined(FUJI_PLATFORM_APPLE2)
#define FUJI_MAX_DISKS 10
#elif defined(FUJI_PLATFORM_ATARI) || defined(FUJI_PLATFORM_C64) \
  || defined(FUJI_PLATFORM_H89)  || defined(FUJI_PLATFORM_LYNX)  \
  || defined(FUJI_PLATFORM_MODEL2) || defined(FUJI_PLATFORM_MSDOS) \
  || defined(FUJI_PLATFORM_MSX)
#define FUJI_MAX_DISKS 8
#else
#error "Unknown number of disk slots"
#endif

#define FILE_MAXLEN    36
#define SSID_MAXLEN    33 /* 32 + NULL */
#define MAX_APPKEY_LEN 64
#define MAX_FILENAME_LEN 256
#define MAX_GUID_LEN 37

// FIXME - use dynamic length
#define MAX_APPKEY_LEN 64

#ifdef FUJI_PLATFORM_C64
#  define MAX_PASSWORD_LEN 65
#else
#  define MAX_PASSWORD_LEN 64
#endif

#endif /* FUJINET_CONST_H */

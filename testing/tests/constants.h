#ifndef CONSTANTS_H
#define CONSTANTS_H

#define MAX_HOSTS 8
#if defined(BUILD_COCO)
#define MAX_DISKS 4
#elif defined(BUILD_APPLE2)
#define MAX_DISKS 10
#else
#define MAX_DISKS 8
#endif

/* Byte offset of the filename within a fuji_read_directory() extended
 * (addtl=0x80) entry. The header preceding the filename is NOT the same
 * size on every bus backend -- each one packs its own
 * set_additional_direntry_details() struct (see fujinet-firmware
 * lib/device/<bus>/*Fuji.cpp):
 *   - rs232 (msdos):   timestamp(6) + size u32(4) + flags(1) + mediatype(1) = 12
 *   - iec (c64):       timestamp(6) + size u16(2) + flags(1) + mediatype(1) = 10
 *   - everyone else:   timestamp(6) + size u32(4) + is_dir(1) + is_trunc(1) + mediatype(1) = 13
 */
#if defined(BUILD_MSDOS)
#define DIRENTRY_FILENAME_OFFSET 12
#elif defined(BUILD_C64)
#define DIRENTRY_FILENAME_OFFSET 10
#else
#define DIRENTRY_FILENAME_OFFSET 13
#endif

#define TNFS_COPY_SOURCE_HOST "tnfs.fujinet.online"

/* Network device specs used across tests */
#define NET_DEVICESPEC  "N1:HTTPS://fujinet.online/"
#define NET_JSON_URL    "N1:HTTPS://httpbin.org/json"
#define NET_POST_URL    "N1:HTTPS://httpbin.org/post"
#define NET_DELETE_URL  "N1:HTTPS://httpbin.org/delete"
#define NET_TCP_SPEC    "N1:TCP://tcpbin.com:4242/"

#endif /* CONSTANTS_H */

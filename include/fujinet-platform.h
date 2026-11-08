#ifndef FUJINET_PLATFORM_H
#define FUJINET_PLATFORM_H

/* FUJI_BUS_CALL_ARGS: the parameter list fuji_bus_call() takes */
#define FUJI_BUS_CALL_ARGS_AUX2     1 /* aux1, aux2, buf, length */
#define FUJI_BUS_CALL_ARGS_AUX4     2 /* aux1..aux4, buf, length */
#define FUJI_BUS_CALL_ARGS_VARIADIC 3 /* only the fields named by the descriptor */

/* FUJI_BUS_CALL_PORTIO: route fuji_bus_call() to fuji_bus_call_portio(), which needs the
   platform's port_*() functions */

/* FUJI_NETWORK_READ_MAX, FUJI_NETWORK_WRITE_MAX: most bytes one network_bus_read() or
   network_bus_write() can move, 0 for no limit */

/* FUJI_NEW_DISK_FORMAT: the NewDisk layout NEW DISK sends */
#define FUJI_NEW_DISK_SECTORS      1 /* NewDiskSectors */
#define FUJI_NEW_DISK_TYPED_BLOCKS 2 /* NewDiskTypedBlocks */
#define FUJI_NEW_DISK_COUNT        3 /* NewDiskCount */
#define FUJI_NEW_DISK_BLOCKS16     4 /* NewDiskBlocks16 */
#define FUJI_NEW_DISK_BLOCKS32     5 /* NewDiskBlocks32 */

/* FUJI_DIR_ENTRY_FORMAT: the FujiDirEntry layout a directory read returns */
#define FUJI_DIR_ENTRY_BOOLS 1 /* FujiDirEntryBools */
#define FUJI_DIR_ENTRY_FLAGS 2 /* FujiDirEntryFlags */

/* FUJI_STATUS_FORMAT: the FNStatus layout STATUS returns */
#define FUJI_STATUS_BYTES 1 /* FNStatusBytes */
#define FUJI_STATUS_IEC   2 /* FNStatusIEC */

/* Every platform sets every switch except FUJI_BUS_CALL_PORTIO; there are no defaults. */
#if defined(_CMOC_VERSION_)
#define FUJI_PLATFORM_COCO
#define FUJI_BUS_CALL_ARGS        FUJI_BUS_CALL_ARGS_VARIADIC
#define FUJI_VARIABLE_LEN_PACKETS 0
#define FUJI_MAX_DISKS            4
#define MAX_PASSWORD_LEN          64
#define FUJI_NETWORK_READ_MAX     0
#define FUJI_NETWORK_WRITE_MAX    0
#define FUJI_NEW_DISK_FORMAT      FUJI_NEW_DISK_COUNT
#define FUJI_DIR_ENTRY_FORMAT     FUJI_DIR_ENTRY_BOOLS
#define FUJI_STATUS_FORMAT        FUJI_STATUS_BYTES

#elif defined(__APPLE2__)
#define FUJI_PLATFORM_APPLE2
#define FUJI_BUS_CALL_ARGS        FUJI_BUS_CALL_ARGS_VARIADIC
#define FUJI_VARIABLE_LEN_PACKETS 1
#define FUJI_MAX_DISKS            10
#define MAX_PASSWORD_LEN          64
#define FUJI_NETWORK_READ_MAX     0
#define FUJI_NETWORK_WRITE_MAX    0
#define FUJI_NEW_DISK_FORMAT      FUJI_NEW_DISK_TYPED_BLOCKS
#define FUJI_DIR_ENTRY_FORMAT     FUJI_DIR_ENTRY_BOOLS
#define FUJI_STATUS_FORMAT        FUJI_STATUS_BYTES

#elif defined(__ATARI__)
#define FUJI_PLATFORM_ATARI
#define FUJI_BUS_CALL_ARGS        FUJI_BUS_CALL_ARGS_AUX2
#define FUJI_VARIABLE_LEN_PACKETS 0
#define FUJI_MAX_DISKS            8
#define MAX_PASSWORD_LEN          64
#define FUJI_NETWORK_READ_MAX     0
#define FUJI_NETWORK_WRITE_MAX    0
#define FUJI_NEW_DISK_FORMAT      FUJI_NEW_DISK_SECTORS
#define FUJI_DIR_ENTRY_FORMAT     FUJI_DIR_ENTRY_BOOLS
#define FUJI_STATUS_FORMAT        FUJI_STATUS_BYTES

#elif defined(__ATARI5200__)
#define FUJI_PLATFORM_ATARI5200
#define FUJI_BUS_CALL_ARGS        FUJI_BUS_CALL_ARGS_AUX4
#define FUJI_VARIABLE_LEN_PACKETS 1
#define FUJI_MAX_DISKS            8
#define MAX_PASSWORD_LEN          64
#define FUJI_NETWORK_READ_MAX     1024 /* the cart mailbox is smaller than a caller's buffer */
#define FUJI_NETWORK_WRITE_MAX    317
#define FUJI_NEW_DISK_FORMAT      FUJI_NEW_DISK_SECTORS
#define FUJI_DIR_ENTRY_FORMAT     FUJI_DIR_ENTRY_FLAGS
#define FUJI_STATUS_FORMAT        FUJI_STATUS_BYTES

#elif defined(__C64__)
#define FUJI_PLATFORM_C64
#define FUJI_BUS_CALL_ARGS        FUJI_BUS_CALL_ARGS_VARIADIC
#define FUJI_VARIABLE_LEN_PACKETS 1
#define FUJI_MAX_DISKS            8
#define MAX_PASSWORD_LEN          65
#define FUJI_NETWORK_READ_MAX     0
#define FUJI_NETWORK_WRITE_MAX    0
#define FUJI_NEW_DISK_FORMAT      FUJI_NEW_DISK_SECTORS
#define FUJI_DIR_ENTRY_FORMAT     FUJI_DIR_ENTRY_FLAGS
#define FUJI_STATUS_FORMAT        FUJI_STATUS_IEC

#elif defined(__MSX__)
#define FUJI_PLATFORM_MSX
#define FUJI_BUS_CALL_ARGS        FUJI_BUS_CALL_ARGS_AUX4
#define FUJI_VARIABLE_LEN_PACKETS 1
#define FUJI_MAX_DISKS            8
#define MAX_PASSWORD_LEN          64
#define FUJI_NETWORK_READ_MAX     0
#define FUJI_NETWORK_WRITE_MAX    0
#define FUJI_NEW_DISK_FORMAT      FUJI_NEW_DISK_SECTORS
#define FUJI_DIR_ENTRY_FORMAT     FUJI_DIR_ENTRY_FLAGS
#define FUJI_STATUS_FORMAT        FUJI_STATUS_BYTES

#elif defined(__LYNX__)
#define FUJI_PLATFORM_LYNX
#define FUJI_BUS_CALL_ARGS        FUJI_BUS_CALL_ARGS_AUX4
#define FUJI_VARIABLE_LEN_PACKETS 1
#define FUJI_MAX_DISKS            8
#define MAX_PASSWORD_LEN          64
#define FUJI_NETWORK_READ_MAX     0
#define FUJI_NETWORK_WRITE_MAX    0
#define FUJI_NEW_DISK_FORMAT      FUJI_NEW_DISK_BLOCKS16
#define FUJI_DIR_ENTRY_FORMAT     FUJI_DIR_ENTRY_BOOLS
#define FUJI_STATUS_FORMAT        FUJI_STATUS_BYTES

#elif defined(__DOS__) || defined(MSDOS)
#define FUJI_PLATFORM_MSDOS
#define FUJI_BUS_CALL_ARGS        FUJI_BUS_CALL_ARGS_AUX4
#define FUJI_VARIABLE_LEN_PACKETS 1
#define FUJI_MAX_DISKS            8
#define MAX_PASSWORD_LEN          64
#define FUJI_NETWORK_READ_MAX     0
#define FUJI_NETWORK_WRITE_MAX    0
#define FUJI_NEW_DISK_FORMAT      FUJI_NEW_DISK_SECTORS
#define FUJI_DIR_ENTRY_FORMAT     FUJI_DIR_ENTRY_FLAGS
#define FUJI_STATUS_FORMAT        FUJI_STATUS_BYTES

#elif defined(__ADAM__) || defined(__COLECOADAM__)
#define FUJI_PLATFORM_ADAM
#define FUJI_BUS_CALL_ARGS        FUJI_BUS_CALL_ARGS_AUX4
#define FUJI_VARIABLE_LEN_PACKETS 1
#define FUJI_MAX_DISKS            4
#define MAX_PASSWORD_LEN          64
#define FUJI_NETWORK_READ_MAX     0
#define FUJI_NETWORK_WRITE_MAX    0
#define FUJI_NEW_DISK_FORMAT      FUJI_NEW_DISK_BLOCKS32
#define FUJI_DIR_ENTRY_FORMAT     FUJI_DIR_ENTRY_BOOLS
#define FUJI_STATUS_FORMAT        FUJI_STATUS_BYTES

#elif defined(__COLECO__)
#define FUJI_PLATFORM_COLECO
#define FUJI_BUS_CALL_ARGS        FUJI_BUS_CALL_ARGS_AUX4
#define FUJI_VARIABLE_LEN_PACKETS 1
#define FUJI_MAX_DISKS            8
#define MAX_PASSWORD_LEN          64
#define FUJI_NETWORK_READ_MAX     1024 /* the cart mailbox is smaller than a caller's buffer */
#define FUJI_NETWORK_WRITE_MAX    317
#define FUJI_NEW_DISK_FORMAT      FUJI_NEW_DISK_SECTORS
#define FUJI_DIR_ENTRY_FORMAT     FUJI_DIR_ENTRY_FLAGS
#define FUJI_STATUS_FORMAT        FUJI_STATUS_BYTES

#elif defined(__NES__)
#define FUJI_PLATFORM_NES
#define FUJI_BUS_CALL_ARGS        FUJI_BUS_CALL_ARGS_AUX4
#define FUJI_VARIABLE_LEN_PACKETS 1
#define FUJI_MAX_DISKS            8
#define MAX_PASSWORD_LEN          64
#define FUJI_NETWORK_READ_MAX     1024 /* the cart mailbox is smaller than a caller's buffer */
#define FUJI_NETWORK_WRITE_MAX    317
#define FUJI_NEW_DISK_FORMAT      FUJI_NEW_DISK_SECTORS
#define FUJI_DIR_ENTRY_FORMAT     FUJI_DIR_ENTRY_FLAGS
#define FUJI_STATUS_FORMAT        FUJI_STATUS_BYTES

#elif defined(__TRS80M2__)
#define FUJI_PLATFORM_MODEL2
#define FUJI_BUS_CALL_ARGS        FUJI_BUS_CALL_ARGS_AUX4
#define FUJI_BUS_CALL_PORTIO
#define FUJI_VARIABLE_LEN_PACKETS 1
#define FUJI_MAX_DISKS            8
#define MAX_PASSWORD_LEN          64
#define FUJI_NETWORK_READ_MAX     0
#define FUJI_NETWORK_WRITE_MAX    0
#define FUJI_NEW_DISK_FORMAT      FUJI_NEW_DISK_SECTORS
#define FUJI_DIR_ENTRY_FORMAT     FUJI_DIR_ENTRY_FLAGS
#define FUJI_STATUS_FORMAT        FUJI_STATUS_BYTES

#elif defined(__H89__) || defined(__HDOS__)
#define FUJI_PLATFORM_H89
#define FUJI_BUS_CALL_ARGS        FUJI_BUS_CALL_ARGS_AUX4
#define FUJI_BUS_CALL_PORTIO
#define FUJI_VARIABLE_LEN_PACKETS 1
#define FUJI_MAX_DISKS            8
#define MAX_PASSWORD_LEN          64
#define FUJI_NETWORK_READ_MAX     0
#define FUJI_NETWORK_WRITE_MAX    0
#define FUJI_NEW_DISK_FORMAT      FUJI_NEW_DISK_SECTORS
#define FUJI_DIR_ENTRY_FORMAT     FUJI_DIR_ENTRY_FLAGS
#define FUJI_STATUS_FORMAT        FUJI_STATUS_BYTES

#elif defined(__PMD85__)
#define FUJI_PLATFORM_PMD85
#define FUJI_BUS_CALL_ARGS        FUJI_BUS_CALL_ARGS_AUX4
#define FUJI_VARIABLE_LEN_PACKETS 0
#define FUJI_MAX_DISKS            4
#define MAX_PASSWORD_LEN          64
#define FUJI_NETWORK_READ_MAX     0
#define FUJI_NETWORK_WRITE_MAX    0
#define FUJI_NEW_DISK_FORMAT      FUJI_NEW_DISK_COUNT
#define FUJI_DIR_ENTRY_FORMAT     FUJI_DIR_ENTRY_BOOLS
#define FUJI_STATUS_FORMAT        FUJI_STATUS_BYTES

#else
#error "Building on unknown platform"
#endif

#ifndef FUJI_BUS_CALL_ARGS
#error "Platform did not set FUJI_BUS_CALL_ARGS"
#endif
#if defined(FUJI_BUS_CALL_PORTIO) && FUJI_BUS_CALL_ARGS != FUJI_BUS_CALL_ARGS_AUX4
#error "FUJI_BUS_CALL_PORTIO needs FUJI_BUS_CALL_ARGS_AUX4"
#endif
#ifndef FUJI_VARIABLE_LEN_PACKETS
#error "Platform did not set FUJI_VARIABLE_LEN_PACKETS"
#endif
#ifndef FUJI_MAX_DISKS
#error "Platform did not set FUJI_MAX_DISKS"
#endif
#ifndef MAX_PASSWORD_LEN
#error "Platform did not set MAX_PASSWORD_LEN"
#endif
#ifndef FUJI_NETWORK_READ_MAX
#error "Platform did not set FUJI_NETWORK_READ_MAX"
#endif
#ifndef FUJI_NETWORK_WRITE_MAX
#error "Platform did not set FUJI_NETWORK_WRITE_MAX"
#endif
#ifndef FUJI_NEW_DISK_FORMAT
#error "Platform did not set FUJI_NEW_DISK_FORMAT"
#endif
#ifndef FUJI_DIR_ENTRY_FORMAT
#error "Platform did not set FUJI_DIR_ENTRY_FORMAT"
#endif
#ifndef FUJI_STATUS_FORMAT
#error "Platform did not set FUJI_STATUS_FORMAT"
#endif

#endif /* FUJINET_PLATFORM_H */

#ifndef FUJINET_PLATFORM_H
#define FUJINET_PLATFORM_H

/* FUJI_BUS_CALL_ARGS: the parameter list fuji_bus_call() takes */
#define FUJI_BUS_CALL_ARGS_AUX2     1 /* aux1, aux2, buf, length */
#define FUJI_BUS_CALL_ARGS_AUX4     2 /* aux1..aux4, buf, length */
#define FUJI_BUS_CALL_ARGS_VARIADIC 3 /* only the fields named by the descriptor */

/* FUJI_NETWORK_READ_MAX, FUJI_NETWORK_WRITE_MAX: most bytes one network_bus_read() or
   network_bus_write() can move, 0 for no limit */

/* Every platform sets every switch; there are no defaults. */
#if defined(_CMOC_VERSION_)
#define FUJI_PLATFORM_COCO
#define FUJI_BUS_CALL_ARGS        FUJI_BUS_CALL_ARGS_VARIADIC
#define FUJI_VARIABLE_LEN_PACKETS 0
#define FUJI_MAX_DISKS            4
#define MAX_PASSWORD_LEN          64
#define FUJI_NETWORK_READ_MAX     0
#define FUJI_NETWORK_WRITE_MAX    0

#elif defined(__APPLE2__)
#define FUJI_PLATFORM_APPLE2
#define FUJI_BUS_CALL_ARGS        FUJI_BUS_CALL_ARGS_VARIADIC
#define FUJI_VARIABLE_LEN_PACKETS 1
#define FUJI_MAX_DISKS            10
#define MAX_PASSWORD_LEN          64
#define FUJI_NETWORK_READ_MAX     0
#define FUJI_NETWORK_WRITE_MAX    0

#elif defined(__ATARI__)
#define FUJI_PLATFORM_ATARI
#define FUJI_BUS_CALL_ARGS        FUJI_BUS_CALL_ARGS_AUX2
#define FUJI_VARIABLE_LEN_PACKETS 0
#define FUJI_MAX_DISKS            8
#define MAX_PASSWORD_LEN          64
#define FUJI_NETWORK_READ_MAX     0
#define FUJI_NETWORK_WRITE_MAX    0

#elif defined(__ATARI5200__)
#define FUJI_PLATFORM_ATARI5200
#define FUJI_BUS_CALL_ARGS        FUJI_BUS_CALL_ARGS_AUX4
#define FUJI_VARIABLE_LEN_PACKETS 1
#define FUJI_MAX_DISKS            8
#define MAX_PASSWORD_LEN          64
#define FUJI_NETWORK_READ_MAX     1024 /* the cart mailbox is smaller than a caller's buffer */
#define FUJI_NETWORK_WRITE_MAX    317

#elif defined(__C64__)
#define FUJI_PLATFORM_C64
#define FUJI_BUS_CALL_ARGS        FUJI_BUS_CALL_ARGS_VARIADIC
#define FUJI_VARIABLE_LEN_PACKETS 1
#define FUJI_MAX_DISKS            8
#define MAX_PASSWORD_LEN          65
#define FUJI_NETWORK_READ_MAX     0
#define FUJI_NETWORK_WRITE_MAX    0

#elif defined(__MSX__)
#define FUJI_PLATFORM_MSX
#define FUJI_BUS_CALL_ARGS        FUJI_BUS_CALL_ARGS_AUX4
#define FUJI_VARIABLE_LEN_PACKETS 1
#define FUJI_MAX_DISKS            8
#define MAX_PASSWORD_LEN          64
#define FUJI_NETWORK_READ_MAX     0
#define FUJI_NETWORK_WRITE_MAX    0

#elif defined(__LYNX__)
#define FUJI_PLATFORM_LYNX
#define FUJI_BUS_CALL_ARGS        FUJI_BUS_CALL_ARGS_AUX4
#define FUJI_VARIABLE_LEN_PACKETS 1
#define FUJI_MAX_DISKS            8
#define MAX_PASSWORD_LEN          64
#define FUJI_NETWORK_READ_MAX     0
#define FUJI_NETWORK_WRITE_MAX    0

#elif defined(__DOS__) || defined(MSDOS)
#define FUJI_PLATFORM_MSDOS
#define FUJI_BUS_CALL_ARGS        FUJI_BUS_CALL_ARGS_AUX4
#define FUJI_VARIABLE_LEN_PACKETS 1
#define FUJI_MAX_DISKS            8
#define MAX_PASSWORD_LEN          64
#define FUJI_NETWORK_READ_MAX     0
#define FUJI_NETWORK_WRITE_MAX    0

#elif defined(__ADAM__) || defined(__COLECOADAM__)
#define FUJI_PLATFORM_ADAM
#define FUJI_BUS_CALL_ARGS        FUJI_BUS_CALL_ARGS_AUX4
#define FUJI_VARIABLE_LEN_PACKETS 1
#define FUJI_MAX_DISKS            4
#define MAX_PASSWORD_LEN          64
#define FUJI_NETWORK_READ_MAX     0
#define FUJI_NETWORK_WRITE_MAX    0

#elif defined(__COLECO__)
#define FUJI_PLATFORM_COLECO
#define FUJI_BUS_CALL_ARGS        FUJI_BUS_CALL_ARGS_AUX4
#define FUJI_VARIABLE_LEN_PACKETS 1
#define FUJI_MAX_DISKS            8
#define MAX_PASSWORD_LEN          64
#define FUJI_NETWORK_READ_MAX     1024 /* the cart mailbox is smaller than a caller's buffer */
#define FUJI_NETWORK_WRITE_MAX    317

#elif defined(__NES__)
#define FUJI_PLATFORM_NES
#define FUJI_BUS_CALL_ARGS        FUJI_BUS_CALL_ARGS_AUX4
#define FUJI_VARIABLE_LEN_PACKETS 1
#define FUJI_MAX_DISKS            8
#define MAX_PASSWORD_LEN          64
#define FUJI_NETWORK_READ_MAX     1024 /* the cart mailbox is smaller than a caller's buffer */
#define FUJI_NETWORK_WRITE_MAX    317

#elif defined(__TRS80M2__)
#define FUJI_PLATFORM_MODEL2
#define FUJI_BUS_CALL_ARGS        FUJI_BUS_CALL_ARGS_AUX4
#define FUJI_VARIABLE_LEN_PACKETS 1
#define FUJI_MAX_DISKS            8
#define MAX_PASSWORD_LEN          64
#define FUJI_NETWORK_READ_MAX     0
#define FUJI_NETWORK_WRITE_MAX    0

#elif defined(__H89__) || defined(__HDOS__)
#define FUJI_PLATFORM_H89
#define FUJI_BUS_CALL_ARGS        FUJI_BUS_CALL_ARGS_AUX4
#define FUJI_VARIABLE_LEN_PACKETS 1
#define FUJI_MAX_DISKS            8
#define MAX_PASSWORD_LEN          64
#define FUJI_NETWORK_READ_MAX     0
#define FUJI_NETWORK_WRITE_MAX    0

#elif defined(__PMD85__)
#define FUJI_PLATFORM_PMD85
#define FUJI_BUS_CALL_ARGS        FUJI_BUS_CALL_ARGS_AUX4
#define FUJI_VARIABLE_LEN_PACKETS 0
#define FUJI_MAX_DISKS            4
#define MAX_PASSWORD_LEN          64
#define FUJI_NETWORK_READ_MAX     0
#define FUJI_NETWORK_WRITE_MAX    0

#else
#error "Building on unknown platform"
#endif

#ifndef FUJI_BUS_CALL_ARGS
#error "Platform did not set FUJI_BUS_CALL_ARGS"
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

#endif /* FUJINET_PLATFORM_H */

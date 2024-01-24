#ifndef FUJI_H
#define FUJI_H

#include <cmoc.h>
#include <coco.h>

// Drivewire Opcodes //////////////////////////////////////////////////////

#define OP_FUJI 0xE2
#define OP_NET  0xE3

// FujiNet Commands ///////////////////////////////////////////////////////

#define FUJICMD_RESET 0xFF                      /* Resets the FujiNet */
#define FUJICMD_GET_SSID 0xFE                   /* Returns WiFi SSID */
#define FUJICMD_SCAN_NETWORKS 0xFD              /* Start WiFi scan */
#define FUJICMD_GET_SCAN_RESULT 0xFC            /* Returns scan results */
#define FUJICMD_SET_SSID 0xFB                   /* Set SSID and Passphrase */
#define FUJICMD_GET_WIFISTATUS 0xFA             /* Check if connected */
#define FUJICMD_MOUNT_HOST 0xF9                 /* Mount host slot */
#define FUJICMD_MOUNT_IMAGE 0xF8                /* Mount disk image */
#define FUJICMD_OPEN_DIRECTORY 0xF7             /*  */
#define FUJICMD_READ_DIR_ENTRY 0xF6             /*  */
#define FUJICMD_CLOSE_DIRECTORY 0xF5            /*  */
#define FUJICMD_READ_HOST_SLOTS 0xF4            /*  */
#define FUJICMD_WRITE_HOST_SLOTS 0xF3           /*  */
#define FUJICMD_READ_DEVICE_SLOTS 0xF2          /*  */
#define FUJICMD_WRITE_DEVICE_SLOTS 0xF1         /*  */
#define FUJICMD_GET_WIFI_ENABLED 0xEA           /* Check if WiFi is enabled or disabled */
#define FUJICMD_UNMOUNT_IMAGE 0xE9              /*  */
#define FUJICMD_GET_ADAPTERCONFIG 0xE8          /*  */
#define FUJICMD_NEW_DISK 0xE7                   /*  */
#define FUJICMD_UNMOUNT_HOST 0xE6               /*  */
#define FUJICMD_GET_DIRECTORY_POSITION 0xE5     /*  */
#define FUJICMD_SET_DIRECTORY_POSITION 0xE4     /*  */
#define FUJICMD_SET_DEVICE_FULLPATH 0xE2        /*  */
#define FUJICMD_WRITE_APPKEY 0xDE               /*  */
#define FUJICMD_READ_APPKEY 0xDD                /*  */
#define FUJICMD_OPEN_APPKEY 0xDC                /*  */
#define FUJICMD_CLOSE_APPKEY 0xDB               /*  */
#define FUJICMD_GET_DEVICE_FULLPATH 0xDA        /*  */
#define FUJICMD_CONFIG_BOOT 0xD9                /*  */
#define FUJICMD_COPY_FILE 0xD8                  /*  */
#define FUJICMD_MOUNT_ALL 0xD7                  /* Mount all disk slots */
#define FUJICMD_SET_BOOT_MODE 0xD6              /*  */
#define FUJICMD_STATUS 0x53                     /*  */
#define FUJICMD_ENABLE_DEVICE 0xD5              /*  */
#define FUJICMD_DISABLE_DEVICE 0xD4             /*  */
#define FUJICMD_RANDOM_NUMBER 0xD3              /*  */
#define FUJICMD_GET_TIME 0xD2                   /*  */
#define FUJICMD_DEVICE_ENABLE_STATUS 0xD1       /*  */
#define FUJICMD_BASE64_ENCODE_INPUT 0xD0
#define FUJICMD_BASE64_ENCODE_COMPUTE 0xCF
#define FUJICMD_BASE64_ENCODE_LENGTH 0xCE
#define FUJICMD_BASE64_ENCODE_OUTPUT 0xCD
#define FUJICMD_BASE64_DECODE_INPUT 0xCC
#define FUJICMD_BASE64_DECODE_COMPUTE 0xCB
#define FUJICMD_BASE64_DECODE_LENGTH 0xCA
#define FUJICMD_BASE64_DECODE_OUTPUT 0xC9
#define FUJICMD_HASH_INPUT 0xC8
#define FUJICMD_HASH_COMPUTE 0xC7
#define FUJICMD_HASH_LENGTH 0xC6
#define FUJICMD_HASH_OUTPUT 0xC5

// Types and Structures ///////////////////////////////////////////////////

#define SSID_MAXLEN 33
#define FILE_MAXLEN 36
/**
 * The current wifi SSID and password
 */
typedef struct
{
  char ssid[SSID_MAXLEN];
  char password[64];
} NetConfig;

/**
 * Returned info for a single SSID entry
 * from a WiFi scan
 */
typedef struct {
  char ssid[SSID_MAXLEN];
  signed char rssi;
} SSIDInfo;

/**
 * @brief typedef for storing host slot information
 */
typedef unsigned char HostSlot[32];

/**
 * @brief Data structure for device slots
 */
typedef struct {
  unsigned char hostSlot;
  unsigned char mode;
  unsigned char file[FILE_MAXLEN];
} DeviceSlot;

/**
 * The current network adapter configuration
 */
typedef struct
{
  char ssid[SSID_MAXLEN];
  char hostname[64];
  unsigned char localIP[4];
  unsigned char gateway[4];
  unsigned char netmask[4];
  unsigned char dnsIP[4];
  unsigned char macAddress[6];
  unsigned char bssid[6];
  char fn_version[15];
} AdapterConfig;

// Interface //////////////////////////////////////////////////////////////

/**
 * @brief Reset the FujiNet
 */
void fuji_reset(void);

/**
 * @brief return the current SSID
 */
BOOL fuji_get_ssid(NetConfig *ssid);

/**
 * @brief Ask FujiNet to scan for WiFi Networks
 * @return # of networks found.
 * @verbose This function will block until completion.
 */
byte fuji_scan_networks(void);

/**
 * @brief Return scan result, after fuji_scan_networks
 * @return TRUE if valid, false if INVALID (e.g. over index)
 * @verbose This function will block until completion.
 */
byte fuji_get_scan_result(byte i, SSIDInfo *s);

/**
 * @brief Set WiFi SSID and Passphrase, and start connection.
 * @param nc Pointer to NetConfig struct with SSID and passkey
 * @return TRUE if successful, FALSE if failed.
 */
BOOL fuji_set_ssid(NetConfig *nc);

/**
 * @brief Return status of WiFi adapter
 * @return WIFI Status byte
 */
byte fuji_get_wifi_status(void);

/**
 * @brief Mount host slot specified by hs (0-3)
 * @param hs the Host slot to mount (0-3)
 */
void fuji_mount_host(byte hs);

/**
 * @brief Mount drive slot specified by ds (0-3) with mode m
 * @param ds the drive slot to mount (0-3)
 * @param m The mode to use (0=r/o 2=r/w)
 */
void fuji_mount_image(byte ds, byte m);

/**
 * @brief Open directory connection to host slot hs, path p, and filter by f
 * @param hs Host slot to open (0-7)
 * @param p Path to open
 * @param f Filter by wildcard
 */
void fuji_open_directory(byte hs, char *p, char *f);

/**
 * @brief Read next directory entry in open directory
 * @param maxlen Maximum length of filename to return (longer names are center-ellipsized)
 * @param a if set to 128, return additional information (such as size and modified date)
 * @param s target string to return directory entry, must be l+1
 */
void fuji_read_directory(byte maxlen, byte a, char *s);

/**
 * @brief Close open directory handle
 */
void fuji_close_directory(void);

/**
 * @brief Read host slots from FujiNet
 * @param h Destination pointer for host slot data.
 */
void fuji_read_host_slots(HostSlot *h);

/**
 * @brief Write host slots from FujiNet
 * @param h source pointer for host slot data.
 */
void fuji_write_host_slots(HostSlot *h);

/**
 * @brief Read device slots from FujiNet
 * @param h Destination pointer for device slot data.
 */
void fuji_read_device_slots(DeviceSlot *d);

/**
 * @brief Write device slots from FujiNet
 * @param h Destination pointer for device slot data.
 */
void fuji_write_device_slots(DeviceSlot *d);

/**
 * @brief Return whether WIFI is enabled (1) or disabled (0)
 * @return 0 = wifi disabled, 1 = wifi enabled
 */
BOOL fuji_get_wifi_enabled(void);

/**
 * @brief Unmount disk image in device slot ds
 * @param ds Device slot to unmount (0-3)
 */
void fuji_unmount_disk_image(byte ds);

/**
 * @brief Get FujiNet Adapter Config and place in AdapterConfig structure pointed to by ac
 * @param ac Pointer to an allocated AdapterConfig structure
 */
void fuji_get_adapter_config(AdapterConfig *ac);

/**
 * @brief Unmount disk image in host slot hs
 * @param hs Host slot to unmount (0-7)
 */
void fuji_unmount_host_slot(byte hs);

/**
 * @brief Get directory position of directory opened by fuji_open_directory()
 * @return integer containing current directory position (0-65535)
 */
unsigned int fuji_get_directory_position(void);

/**
 * @brief Set directory position of directory opened by fuji_open_directory()
 * @param pos integer containing new directory position (0-65535)
 */
void fuji_set_directory_position(unsigned int pos);

/**
 * @brief Return device filename to string pointer s (needs to be 256 bytes!)
 * @param ds the device slot to return (0-3)
 * @param s Destination pointer
 */
void fuji_set_device_filename(byte ds, char *s);

#endif /* FUJI_H */

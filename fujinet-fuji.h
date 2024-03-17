#ifndef FN_FUJI_H
#define FN_FUJI_H

// TODO: this header file needs more documentation

// In general, bools return the "success" status, so true is good, false is bad.

#include <stdint.h>
#include <stdbool.h>

#define FILE_MAXLEN 36
#define SSID_MAXLEN 33 /* 32 + NULL */
#define MAX_APPKEY_LEN 64

enum WifiStatus {
    no_ssid_available   = 1,
    connected           = 3,
    connect_failed      = 4,
    connection_lost     = 5
};

typedef struct {
  char ssid[SSID_MAXLEN];
  signed char rssi;
} SSIDInfo;

typedef struct
{
  char ssid[SSID_MAXLEN];
  char password[64];
} NetConfig;

typedef struct
{
  char ssid[SSID_MAXLEN];
  char hostname[64];
  uint8_t localIP[4];
  uint8_t gateway[4];
  uint8_t netmask[4];
  uint8_t dnsIP[4];
  uint8_t macAddress[6];
  uint8_t bssid[6];
  char fn_version[15];
} AdapterConfig;

typedef struct
{
  char ssid[SSID_MAXLEN];
  char hostname[64];
  uint8_t localIP[4];
  uint8_t gateway[4];
  uint8_t netmask[4];
  uint8_t dnsIP[4];
  uint8_t macAddress[6];
  uint8_t bssid[6];
  char fn_version[15];
  char sLocalIP[16];
  char sGateway[16];
  char sNetmask[16];
  char sDnsIP[16];
  char sMacAddress[18];
  char sBssid[18];
} AdapterConfigExtended;

typedef uint8_t HostSlot[32];

typedef struct {
  uint8_t hostSlot;
  uint8_t mode;
  uint8_t file[FILE_MAXLEN];
} DeviceSlot;


// Disks have different structures / parameters

#ifdef BUILD_ATARI
typedef struct
{
  uint16_t numSectors;
  uint16_t sectorSize;
  uint8_t hostSlot;
  uint8_t deviceSlot;
  char filename[256];
} NewDisk;
#endif

#ifdef BUILD_APPLE2
typedef struct
{
  uint8_t hostSlot;
  uint8_t deviceSlot;
  uint8_t createType;
  uint32_t numBlocks;
  char filename[256];
} NewDisk;

#endif

typedef struct
{
    unsigned int creator;
    unsigned char app;
    unsigned char key;
    unsigned char mode;
    unsigned char reserved;
} AppKeyOpen;

typedef struct
{
    unsigned int length;
    unsigned char value[MAX_APPKEY_LEN];
} AppKeyRead;

typedef struct
{
    unsigned char value[MAX_APPKEY_LEN];
} AppKeyWrite;

typedef union {
    AppKeyOpen open;
    AppKeyRead read;
    AppKeyWrite write;
} AppKeyDataBlock;

typedef struct
{
    unsigned char value[4];
} FNStatus;

enum HashType {
    MD5,
    SHA1,
    SHA256,
    SHA512
};

void fuji_close_directory(void);
void fuji_copy_file(uint8_t src_slot, uint8_t dst_slot, char *copy_spec);
void fuji_create_new(NewDisk *new_disk);
void fuji_disable_device(uint8_t d);
bool fuji_error();
void fuji_enable_device(uint8_t d);
void fuji_enable_udpstream(uint16_t port, char *host);

/*
 * Gets adapter config information from FN, e.g. IP, MAC, BSSID etc.
 * Raw version that returns bytes for all IP etc related values.
 * @return Success status, true if all OK.
 */
bool fuji_get_adapter_config(AdapterConfig *ac);


void fuji_get_adapter_config_extended(AdapterConfigExtended *ac);



bool fuji_get_device_enabled_status(uint8_t d);
void fuji_get_device_filename(uint8_t ds, char *buffer);
void fuji_get_device_slots(DeviceSlot *d);
uint16_t fuji_get_directory_position();
void fuji_get_host_slots(HostSlot *h);
void fuji_get_host_prefix(uint8_t hs, char *prefix);
uint8_t fuji_get_hsio_index();
void fuji_get_scan_result(uint8_t n, SSIDInfo *ssid_info);
void fuji_get_ssid(NetConfig *net_config);
bool fuji_get_wifi_enabled(void);
uint8_t fuji_get_wifi_status(void);

/*
 * Mount all devices
 * @return true if successful, false otherwise
 */
bool fuji_mount_all(void);

/*
 * Mount specific device slot
 * @return true if successful, false otherwise
 */
bool fuji_mount_disk_image(uint8_t ds, uint8_t mode);


/*
 * Mount specific host slot
 * @return true if successful, false otherwise
 */
bool fuji_mount_host_slot(uint8_t hs);

/*
 * Open the given directory on the given host slot
 * @return true if successful, false otherwise
 */
bool fuji_open_directory(uint8_t hs, char *path_filter);

/*
 * Save all device slots to FN
 * @return true if successful, false if there was an error from FN
 */
bool fuji_put_device_slots(DeviceSlot *d);

/*
 * Save all hosts slots to FN
 * @return true if successful, false if there was an error from FN
 */
bool fuji_put_host_slots(HostSlot *h);

char *fuji_read_directory(uint8_t maxlen, uint8_t aux2, char *buffer);

/*
 * Reset FN
 * @return true if successful, false if there was an error from FN
 */
bool fuji_reset(void);

uint8_t fuji_scan_for_networks(void);
void fuji_set_boot_config(uint8_t toggle);
void fuji_set_boot_mode(uint8_t mode);
void fuji_set_device_filename(uint8_t mode, uint8_t hs, uint8_t ds, char *buffer);
void fuji_set_sio_external_clock(uint16_t rate);
void fuji_set_directory_position(uint16_t pos);
void fuji_set_hsio_index(bool save, uint8_t index);
void fuji_set_host_prefix(uint8_t hs, char *prefix);
void fuji_set_ssid(NetConfig *nc);
void fuji_status(FNStatus *status);
void fuji_unmount_disk_image(uint8_t ds);
void fuji_unmount_host_slot(uint8_t hs);

// app key
uint8_t fuji_appkey_open(AppKeyOpen *buffer);
uint8_t fuji_appkey_read(AppKeyRead *buffer);
uint8_t fuji_appkey_write(uint16_t count, AppKeyWrite *buffer);

// Base64
uint8_t fuji_base64_decode_compute();
uint8_t fuji_base64_decode_input(char *s, uint16_t len);
uint8_t fuji_base64_decode_length(unsigned long *len);
uint8_t fuji_base64_decode_output(char *s, uint16_t len);

uint8_t fuji_base64_encode_compute();
uint8_t fuji_base64_encode_input(char *s, uint16_t len);
uint8_t fuji_base64_encode_length(unsigned long *len);
uint8_t fuji_base64_encode_output(char *s, uint16_t len);

// Hash
uint8_t fuji_hash_compute(uint8_t type);
uint8_t fuji_hash_input(char *s, uint16_t len);
uint8_t fuji_hash_length(uint8_t mode);
uint8_t fuji_hash_output(uint8_t output_type, char *s, uint16_t len);

#endif /* FN_FUJI_H */
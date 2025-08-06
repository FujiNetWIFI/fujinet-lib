/* Wrappers for functions that take a pointer. Can't directly use the
   macro, otherwise the C compiler won't do any type checking on
   the pointers. */

#include "fujinet-fuji.h"

bool fuji_copy_file(uint8_t src_slot, uint8_t dst_slot, char *copy_spec)
{
  return FUJICALL_A1_A2_D(FUJICMD_COPY_FILE, src_slot, dst_slot, copy_spec, MAX_FILENAME_LEN);
}

bool fuji_create_new(NewDisk *new_disk)
{
  return FUJICALL_D(FUJICMD_NEW_DISK, new_disk, sizeof(new_disk));
}

bool fuji_get_adapter_config_extended(AdapterConfigExtended *ac)
{
  return FUJICALL_RV(FUJICMD_GET_ADAPTERCONFIG_EXTENDED, ac, sizeof(AdapterConfigExtended));
}

bool fuji_get_device_filename(uint8_t ds, char *buffer)
{
  return FUJICALL_RV(FUJICMD_GET_DEVICE1_FULLPATH + ds, buffer, MAX_FILENAME_LEN);
}

bool fuji_get_device_slots(DeviceSlot *d, size_t count)
{
  return FUJICALL_RV(FUJICMD_READ_DEVICE_SLOTS, d, sizeof(DeviceSlot) * count);
}

bool fuji_get_host_prefix(uint8_t hs, char *prefix)
{
  return FUJICALL_A1_RV(FUJICMD_GET_HOST_PREFIX, hs, prefix, MAX_FILENAME_LEN);
}

bool fuji_get_host_slots(HostSlot *h, size_t count)
{
  return FUJICALL_RV(FUJICMD_READ_HOST_SLOTS, h, sizeof(HostSlot) * count);
}

bool fuji_get_scan_result(uint8_t n, SSIDInfo *ssid_info)
{
  return FUJICALL_A1_RV(FUJICMD_GET_SCAN_RESULT, n, ssid_info, sizeof(SSIDInfo));
}

bool fuji_get_ssid(NetConfig *net_config)
{
  return FUJICALL_RV(FUJICMD_GET_SSID, net_config, sizeof(NetConfig));
}

bool fuji_get_wifi_status(uint8_t *status)
{
  return FUJICALL_RV(FUJICMD_GET_WIFISTATUS, status, 1);
}

bool fuji_open_directory(uint8_t hs, char *path_filter)
{
  return FUJICALL_A1_D(FUJICMD_OPEN_DIRECTORY, hs, path_filter, MAX_FILENAME_LEN);
}

bool fuji_put_device_slots(DeviceSlot *d, size_t count)
{
  return FUJICALL_D(FUJICMD_WRITE_DEVICE_SLOTS, d, sizeof(DeviceSlot) * count);
}

bool fuji_put_host_slots(HostSlot *h, size_t count)
{
  return FUJICALL_D(FUJICMD_WRITE_HOST_SLOTS, h, sizeof(HostSlot) * count);
}

bool fuji_read_directory(uint8_t maxlen, uint8_t aux2, char *buffer)
{
  return FUJICALL_A1_A2_RV(FUJICMD_READ_DIR_ENTRY, maxlen, aux2, buffer, maxlen);
}

bool fuji_scan_for_networks(uint8_t *count)
{
  return FUJICALL_RV(FUJICMD_SCAN_NETWORKS, count, 1);
}

bool fuji_set_device_filename(uint8_t mode, uint8_t hs, uint8_t ds, char *buffer)
{
#ifdef __ATARI__
  return FUJICALL_A1_A2_D(FUJICMD_SET_DEVICE_FULLPATH, ds, (hs << 4) | mode,
			  buffer, MAX_FILENAME_LEN);
#else /* !__ATARI__ */
  return FUJICALL_A1_A2_A3_D(FUJICMD_SET_DEVICE_FULLPATH, ds, hs, mode,
			     buffer, MAX_FILENAME_LEN);
#endif /* __ATARI__ */
}

bool fuji_set_host_prefix(uint8_t hs, char *prefix)
{
  return FUJICALL_A1_D(FUJICMD_SET_HOST_PREFIX, hs, prefix, MAX_FILENAME_LEN);
}

bool fuji_set_ssid(NetConfig *nc)
{
  return FUJICALL_D(FUJICMD_SET_SSID, nc, sizeof(NetConfig));
}

bool fuji_status(FNStatus *status)
{
  return FUJICALL_RV(FUJICMD_STATUS, status, sizeof(FNStatus));
}

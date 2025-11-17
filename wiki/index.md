# FujiNet-lib API Reference

This documentation is generated from the root header files:

- `fujinet-fuji.h`
- `fujinet-network.h`
- `fujinet-clock.h`

## Headers and Functions

### `fujinet-fuji.h`

Functions declared in this header:

- [`fuji_close_directory`](generated/fuji_close_directory.md)
- [`fuji_copy_file`](generated/fuji_copy_file.md)
- [`fuji_create_new`](generated/fuji_create_new.md)
- [`fuji_disable_device`](generated/fuji_disable_device.md)
- [`fuji_enable_device`](generated/fuji_enable_device.md)
- [`fuji_enable_udpstream`](generated/fuji_enable_udpstream.md)
- [`fuji_error`](generated/fuji_error.md)
- [`fuji_get_adapter_config`](generated/fuji_get_adapter_config.md)
- [`fuji_get_adapter_config_extended`](generated/fuji_get_adapter_config_extended.md)
- [`fuji_get_device_enabled_status`](generated/fuji_get_device_enabled_status.md)
- [`fuji_get_device_filename`](generated/fuji_get_device_filename.md)
- [`fuji_get_device_slots`](generated/fuji_get_device_slots.md)
- [`fuji_get_directory_position`](generated/fuji_get_directory_position.md)
- [`fuji_get_host_prefix`](generated/fuji_get_host_prefix.md)
- [`fuji_get_host_slots`](generated/fuji_get_host_slots.md)
- [`fuji_get_scan_result`](generated/fuji_get_scan_result.md)
- [`fuji_get_ssid`](generated/fuji_get_ssid.md)
- [`fuji_get_wifi_enabled`](generated/fuji_get_wifi_enabled.md)
- [`fuji_get_wifi_status`](generated/fuji_get_wifi_status.md)
- [`fuji_mount_all`](generated/fuji_mount_all.md)
- [`fuji_mount_disk_image`](generated/fuji_mount_disk_image.md)
- [`fuji_mount_host_slot`](generated/fuji_mount_host_slot.md)
- [`fuji_open_directory`](generated/fuji_open_directory.md)
- [`fuji_open_directory2`](generated/fuji_open_directory2.md)
- [`fuji_put_device_slots`](generated/fuji_put_device_slots.md)
- [`fuji_put_host_slots`](generated/fuji_put_host_slots.md)
- [`fuji_read_directory`](generated/fuji_read_directory.md)
- [`fuji_read_directory_block`](generated/fuji_read_directory_block.md)
- [`fuji_reset`](generated/fuji_reset.md)
- [`fuji_scan_for_networks`](generated/fuji_scan_for_networks.md)
- [`fuji_set_boot_config`](generated/fuji_set_boot_config.md)
- [`fuji_set_boot_mode`](generated/fuji_set_boot_mode.md)
- [`fuji_set_device_filename`](generated/fuji_set_device_filename.md)
- [`fuji_set_directory_position`](generated/fuji_set_directory_position.md)
- [`fuji_set_hsio_index`](generated/fuji_set_hsio_index.md)
- [`fuji_set_sio_external_clock`](generated/fuji_set_sio_external_clock.md)
- [`fuji_set_ssid`](generated/fuji_set_ssid.md)
- [`fuji_status`](generated/fuji_status.md)
- [`fuji_unmount_host_slot`](generated/fuji_unmount_host_slot.md)
- [`fuji_read_appkey`](generated/fuji_read_appkey.md)
- [`fuji_write_appkey`](generated/fuji_write_appkey.md)
- [`fuji_set_appkey_details`](generated/fuji_set_appkey_details.md)
- [`fuji_base64_decode_compute`](generated/fuji_base64_decode_compute.md)
- [`fuji_base64_decode_input`](generated/fuji_base64_decode_input.md)
- [`fuji_base64_decode_length`](generated/fuji_base64_decode_length.md)
- [`fuji_base64_decode_output`](generated/fuji_base64_decode_output.md)
- [`fuji_base64_encode_compute`](generated/fuji_base64_encode_compute.md)
- [`fuji_base64_encode_input`](generated/fuji_base64_encode_input.md)
- [`fuji_base64_encode_length`](generated/fuji_base64_encode_length.md)
- [`fuji_base64_encode_output`](generated/fuji_base64_encode_output.md)
- [`fuji_hash_compute`](generated/fuji_hash_compute.md)
- [`fuji_hash_compute_no_clear`](generated/fuji_hash_compute_no_clear.md)
- [`fuji_hash_input`](generated/fuji_hash_input.md)
- [`fuji_hash_output`](generated/fuji_hash_output.md)
- [`fuji_hash_length`](generated/fuji_hash_length.md)
- [`fuji_hash_size`](generated/fuji_hash_size.md)
- [`fuji_hash_data`](generated/fuji_hash_data.md)
- [`fuji_hash_clear`](generated/fuji_hash_clear.md)
- [`fuji_hash_add`](generated/fuji_hash_add.md)
- [`fuji_hash_calculate`](generated/fuji_hash_calculate.md)

### `fujinet-network.h`

Functions declared in this header:

- [`fn_error`](generated/fn_error.md)
- [`network_init`](generated/network_init.md)
- [`network_status`](generated/network_status.md)
- [`network_close`](generated/network_close.md)
- [`network_open`](generated/network_open.md)
- [`network_read_nb`](generated/network_read_nb.md)
- [`network_read`](generated/network_read.md)
- [`network_write`](generated/network_write.md)
- [`network_ioctl`](generated/network_ioctl.md)
- [`network_json_parse`](generated/network_json_parse.md)
- [`network_json_query`](generated/network_json_query.md)
- [`network_http_set_channel_mode`](generated/network_http_set_channel_mode.md)
- [`network_http_start_add_headers`](generated/network_http_start_add_headers.md)
- [`network_http_end_add_headers`](generated/network_http_end_add_headers.md)
- [`network_http_add_header`](generated/network_http_add_header.md)
- [`network_http_post`](generated/network_http_post.md)
- [`network_http_post_bin`](generated/network_http_post_bin.md)
- [`network_http_put`](generated/network_http_put.md)
- [`network_http_delete`](generated/network_http_delete.md)
- [`network_unit`](generated/network_unit.md)
- [`network_fs_delete`](generated/network_fs_delete.md)
- [`network_fs_rename`](generated/network_fs_rename.md)
- [`network_fs_lock`](generated/network_fs_lock.md)
- [`network_fs_unlock`](generated/network_fs_unlock.md)
- [`network_fs_mkdir`](generated/network_fs_mkdir.md)
- [`network_fs_rmdir`](generated/network_fs_rmdir.md)
- [`network_fs_cd`](generated/network_fs_cd.md)

### `fujinet-clock.h`

Functions declared in this header:

- [`clock_set_tz`](generated/clock_set_tz.md)
- [`clock_get_tz`](generated/clock_get_tz.md)
- [`clock_get_time`](generated/clock_get_time.md)
- [`clock_get_time_tz`](generated/clock_get_time_tz.md)


AR=lwar
CC=cmoc

OBJS=fuji_reset.o \
	fuji_get_ssid.o \
	fuji_scan_networks.o \
	fuji_get_scan_result.o \
	fuji_set_ssid.o \
	fuji_get_wifi_status.o \
	fuji_mount_host.o \
	fuji_mount_image.o \
	fuji_open_directory.o \
	fuji_read_directory_entry.o \
	fuji_close_directory.o \
	fuji_read_host_slots.o \
	fuji_write_host_slots.o \
	fuji_read_device_slots.o \
	fuji_write_device_slots.o \
	fuji_get_wifi_enabled.o \
	fuji_unmount_image.o \
	fuji_get_adapter_config.o

libfuji.a: $(OBJS)
	$(AR) --create libfuji.a $(OBJS)

fuji_reset.o: fuji_reset.c fuji.h
	$(CC) -o fuji_reset.o -c fuji_reset.c

fuji_get_ssid.o: fuji_get_ssid.c fuji.h
	$(CC) -o fuji_get_ssid.o -c fuji_get_ssid.c

fuji_scan_networks.o: fuji_scan_networks.c fuji.h
	$(CC) -o fuji_scan_networks.o -c fuji_scan_networks.c

fuji_get_scan_result.o: fuji_get_scan_result.c fuji.h
	$(CC) -o fuji_get_scan_result.o -c fuji_get_scan_result.c

fuji_set_ssid.o: fuji_set_ssid.c fuji.h
	$(CC) -o fuji_set_ssid.o -c fuji_set_ssid.c

fuji_get_wifi_status.o: fuji_get_wifi_status.c fuji.h
	$(CC) -o fuji_get_wifi_status.o -c fuji_get_wifi_status.c

fuji_mount_host.o: fuji_mount_host.c fuji.h
	$(CC) -o fuji_mount_host.o -c fuji_mount_host.c

fuji_mount_image.o: fuji_mount_image.c fuji.h
	$(CC) -o fuji_mount_image.o -c fuji_mount_image.c

fuji_open_directory.o: fuji_open_directory.c fuji.h
	$(CC) -o fuji_open_directory.o -c fuji_open_directory.c

fuji_read_directory_entry.o: fuji_read_directory_entry.c fuji.h
	$(CC) -o fuji_read_directory_entry.o -c fuji_read_directory_entry.c

fuji_close_directory.o: fuji_close_directory.c fuji.h
	$(CC) -o fuji_close_directory.o -c fuji_close_directory.c

fuji_read_host_slots.o: fuji_read_host_slots.c fuji.h
	$(CC) -o fuji_read_host_slots.o -c fuji_read_host_slots.c

fuji_write_host_slots.o: fuji_write_host_slots.c fuji.h
	$(CC) -o fuji_write_host_slots.o -c fuji_write_host_slots.c

fuji_read_device_slots.o: fuji_read_device_slots.c fuji.h
	$(CC) -o fuji_read_device_slots.o -c fuji_read_device_slots.c

fuji_write_device_slots.o: fuji_write_device_slots.c fuji.h
	$(CC) -o fuji_write_device_slots.o -c fuji_write_device_slots.c

jfuji_get_wifi_enabled.o: fuji_get_wifi_enabled.c fuji.h
	$(CC) -o fuji_get_wifi_enabled.o -c fuji_get_wifi_enabled.c

fuji_unmount_image.o: fuji_unmount_image.c fuji.h
	$(CC) -o fuji_unmount_image.o -c fuji_unmount_image.c

fuji_get_adapter_config.o: fuji_get_adapter_config.c fuji.h
	$(CC) -o fuji_get_adapter_config.o -c fuji_get_adapter_config.c

clean:
	$(RM) *.o *.a *~

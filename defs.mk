# FUJICMDS := fuji_appkey_open_read.c fuji_appkey_open_write.c		\
# 	    fuji_close_directory.c fuji_copy_file.c fuji_create_new.c	\
# 	    fuji_data.c fuji_disable_device.c fuji_enable_device.c	\
# 	    fuji_enable_udpstream.c fuji_error.c			\
# 	    fuji_get_adapter_config.c					\
# 	    fuji_get_adapter_config_extended.c				\
# 	    fuji_get_device_enabled_status.c				\
# 	    fuji_get_device_filename.c fuji_get_device_slots.c		\
# 	    fuji_get_directory_position.c fuji_get_host_prefix.c	\
# 	    fuji_get_host_slots.c fuji_get_hsio_index.c			\
# 	    fuji_get_scan_result.c fuji_get_ssid.c			\
# 	    fuji_get_wifi_enabled.c fuji_get_wifi_status.c		\
# 	    fuji_hash_clear.c fuji_hash_compute.c			\
# 	    fuji_hash_compute_no_clear.c fuji_hash_input.c		\
# 	    fuji_hash_length.c fuji_hash_output.c fuji_mount_all.c	\
# 	    fuji_mount_disk_image.c fuji_mount_host_slot.c		\
# 	    fuji_open_directory2.c fuji_open_directory.c		\
# 	    fuji_put_device_slots.c fuji_put_host_slots.c		\
# 	    fuji_read_directory.c fuji_reset.c				\
# 	    fuji_scan_for_networks.c fuji_set_boot_config.c		\
# 	    fuji_set_boot_mode.c fuji_set_device_filename.c		\
# 	    fuji_set_directory_position.c fuji_set_host_prefix.c	\
# 	    fuji_set_hsio_index.c fuji_set_ssid.c fuji_set_status.c	\
# 	    fuji_status.c fuji_unmount_disk_image.c			\
# 	    fuji_unmount_host_slot.c
FUJICMDS := fuji_get_adapter_config_extended.c
FUJIBUS := transaction.c
AFILES_SP := bus/apple2/sp_close_nw.s bus/apple2/sp_close.s		\
	     bus/apple2/sp_clr_payload.s bus/apple2/sp_data.s		\
	     bus/apple2/sp_find_clock.s bus/apple2/sp_find_cpm.s	\
	     bus/apple2/sp_find_device.s bus/apple2/sp_find_fuji.s	\
	     bus/apple2/sp_find_modem.s bus/apple2/sp_find_network.s	\
	     bus/apple2/sp_find_printer.s bus/apple2/sp_init.s		\
	     bus/apple2/sp_open_nw.s bus/apple2/sp_open.s		\
	     bus/apple2/sp_read_nw.s bus/apple2/sp_read.s		\
	     bus/apple2/sp_rw_common.s					\
	     bus/apple2/sp_status_control_dispatch.s			\
	     bus/apple2/sp_write_nw.s bus/apple2/sp_write.s
HFILES := $(addprefix include/, fujinet-fuji.h fujinet-bus.h)
CFILES = $(addprefix common/, $(FUJICMDS))
AFILES =
CFILES_A2 = $(addprefix bus/apple2/, $(FUJIBUS))
AFILES_A2 = $(addprefix bus/apple2/, $(AFILES_SP))
BUILDDIR = build
OBJDIR := $(PLATFORM)_obj
OBJS = $(addprefix $(OBJDIR)/, $(notdir $(CFILES:.c=.o) $(AFILES:.s=.o)))
OBJS_A2 = $(addprefix $(OBJDIR)/, $(notdir $(CFILES_A2:.c=.o) $(AFILES_A2:.s=.o)))

vpath %.c common bus/apple2
vpath %.s common bus/apple2

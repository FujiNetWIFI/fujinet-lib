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
FUJICMDS := fuji_get_adapter_config_extended.c fn_data.c	\
	     fuji_get_host_prefix.c fuji_get_device_slots.c	\
	     fuji_get_host_slots.c fuji_read_directory.c
FUJIBUS := fujinet-bus.c
AFILES_SP := sp_close_nw.s sp_close.s sp_clr_payload.s sp_data.s	\
	     sp_find_clock.s sp_find_cpm.s sp_find_device.s		\
	     sp_find_fuji.s sp_find_modem.s sp_find_network.s		\
	     sp_find_printer.s sp_init.s sp_open_nw.s sp_open.s		\
	     sp_read_nw.s sp_read.s sp_rw_common.s			\
	     sp_status_control_dispatch.s sp_write_nw.s sp_write.s
CFILES_DW := bus_ready.c dwread.c dwwrite.c fuji_get_error.c	\
	     fuji_get_response.c network_get_error.c		\
	     network_get_response.c fn_error.c
AFILES_CBM := fuji_cbm_open.s
HFILES := $(addprefix include/, fujinet-fuji.h fujinet-bus.h)
CFILES = $(addprefix common/, $(FUJICMDS))
AFILES =
CFILES_A2 = $(addprefix bus/apple2/, $(FUJIBUS))
AFILES_A2 = $(addprefix bus/apple2/, $(AFILES_SP))
CFILES_COCO = $(addprefix bus/coco/, $(FUJIBUS) $(CFILES_DW))
CFILES_C64 = $(addprefix bus/c64/, $(FUJIBUS))
AFILES_C64 = $(addprefix bus/c64/, $(AFILES_CBM))
BUILDDIR = build
OBJDIR := $(PLATFORM)_obj
OBJS = $(addprefix $(OBJDIR)/, $(notdir $(CFILES:.c=.o) $(AFILES:.s=.o)))
OBJS_A2 = $(addprefix $(OBJDIR)/, $(notdir $(CFILES_A2:.c=.o) $(AFILES_A2:.s=.o)))
OBJS_COCO = $(addprefix $(OBJDIR)/, $(notdir $(CFILES_COCO:.c=.o) $(AFILES_COCO:.s=.o)))
OBJS_C64 = $(addprefix $(OBJDIR)/, $(notdir $(CFILES_C64:.c=.o) $(AFILES_C64:.s=.o)))

vpath %.c common
vpath %.s common

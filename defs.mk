FUJICMDS := fuji_appkey.c network.c network_json.c network_unit_status_default.c
AFILES_SP := fn_error.s sp_data.s sp_find_clock.s sp_find_cpm.s		\
	     sp_find_device.s sp_find_fuji.s sp_find_modem.s		\
	     sp_find_network.s sp_find_printer.s sp_init.s		\
	     sp_read_nw.s sp_read.s sp_rw_common.s			\
	     sp_status_control_dispatch.s sp_write_nw.s sp_write.s
CFILES_DW := bus_ready.c dwread.c dwwrite.c fuji_get_error.c	\
	     fuji_get_response.c network_get_error.c		\
	     network_get_response.c fn_error.c
AFILES_CBM := fuji_cbm_open.s
AFILES_SIO := call_sio.s
HFILES := $(addprefix include/, fujinet-fuji.h fujinet-bus.h)
CFILES = $(addprefix common/, $(FUJICMDS))
AFILES =
CFILES_A2 = $(addprefix bus/apple2/, fujinet-bus-apple2.c)
AFILES_A2 = $(addprefix bus/apple2/, $(AFILES_SP))
CFILES_COCO = $(addprefix bus/coco/, fujinet-bus-coco.c $(CFILES_DW))
CFILES_C64 = $(addprefix bus/c64/, fujinet-bus-c64.c network_unit_status.c)
AFILES_C64 = $(addprefix bus/c64/, $(AFILES_CBM))
CFILES_ATARI = $(addprefix bus/atari/, fujinet-bus-atari.c)
AFILES_ATARI = $(addprefix bus/atari/, $(AFILES_SIO))
BUILDDIR = build
OBJDIR := $(PLATFORM)_obj
OBJS_COMMON = $(addprefix $(OBJDIR)/, $(notdir $(CFILES:.c=.o) $(AFILES:.s=.o)))
OBJS_A2 = $(OBJS_COMMON) \
	$(addprefix $(OBJDIR)/, $(notdir $(CFILES_A2:.c=.o) $(AFILES_A2:.s=.o)))
OBJS_COCO = $(OBJS_COMMON) \
	$(addprefix $(OBJDIR)/, $(notdir $(CFILES_COCO:.c=.o) $(AFILES_COCO:.s=.o)))
OBJS_C64 = $(filter-out common/network_unit_status_default.o,$(OBJS_COMMON)) \
	$(addprefix $(OBJDIR)/, $(notdir $(CFILES_C64:.c=.o) $(AFILES_C64:.s=.o)))
OBJS_ATARI = $(OBJS_COMMON) \
	$(addprefix $(OBJDIR)/, $(notdir $(CFILES_ATARI:.c=.o) $(AFILES_ATARI:.s=.o)))

vpath %.c common
vpath %.s common

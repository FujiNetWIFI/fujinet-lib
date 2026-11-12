EXEC_SUFFIX = .rom
LIBRARY = $(R2R_PD)/$(PRODUCT_BASE).$(PLATFORM).lib

MWD := $(realpath $(dir $(lastword $(MAKEFILE_LIST)))..)
include $(MWD)/common.mk
include $(MWD)/toolchains/z88dk.mk

# The FujiNet cartridge owns $F800-$FFFF: fence code and data below it, make the image exactly 32K.
COLECO_FLAGS = +coleco
COLECO_LDFLAGS = -Cz--romsize=32768 -Cz--rombase=32768 \
  -Cz--code-fence=0xF800 -Cz--data-fence=0xF800
CFLAGS += $(COLECO_FLAGS)
LDFLAGS += $(COLECO_FLAGS)
ifneq ($(IS_LIBRARY),1)
  LDFLAGS += $(COLECO_LDFLAGS) -create-app
endif

r2r:: $(BUILD_EXEC) $(BUILD_LIB) $(R2R_EXTRA_DEPS)
	make -f $(PLATFORM_MK) $(PLATFORM)/r2r-post

# The "FUJI" claim the cartridge looks for, at image offset $7CFC.
$(PLATFORM)/executable-post::
	printf FUJI | dd of=$(EXECUTABLE) bs=1 seek=$$((0x7CFC)) conv=notrunc status=none

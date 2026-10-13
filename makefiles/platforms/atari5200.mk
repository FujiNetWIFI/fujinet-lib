EXEC_SUFFIX = .bin
LIBRARY = $(R2R_PD)/$(PRODUCT_BASE).$(PLATFORM).lib

MWD := $(realpath $(dir $(lastword $(MAKEFILE_LIST)))..)
include $(MWD)/common.mk
include $(MWD)/toolchains/cc65.mk

# A 5200 FujiNet client is a 32K image at $4000-$BFFF around the cart's
# mailbox at $B000, started by the lib's own crt0 (cc65's relies on the
# BIOS, which the client's header skips). The romstamp checks the claim and
# the mailbox's rules; MAME's -cart takes the .bin as it is.
ATARI5200_CFG ?= $(MWD)/atari5200-fujinet.cfg
ATARI5200_MAP ?= $(EXECUTABLE:.bin=.map)

ifneq ($(IS_LIBRARY),1)
  LDFLAGS += -C $(ATARI5200_CFG) -m $(ATARI5200_MAP)

.DELETE_ON_ERROR:

$(PLATFORM)/executable-post::
	$(MWD)/atari5200-romstamp.py --map $(ATARI5200_MAP) $(EXECUTABLE)
endif

r2r:: $(BUILD_EXEC) $(BUILD_LIB) $(R2R_EXTRA_DEPS)
	make -f $(PLATFORM_MK) $(PLATFORM)/r2r-post

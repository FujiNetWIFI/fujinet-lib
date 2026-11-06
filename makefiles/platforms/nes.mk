EXEC_SUFFIX = .nes
LIBRARY = $(R2R_PD)/$(PRODUCT_BASE).$(PLATFORM).lib

MWD := $(realpath $(dir $(lastword $(MAKEFILE_LIST)))..)
include $(MWD)/common.mk
include $(MWD)/toolchains/cc65.mk

# A NES FujiNet client is cc65's stock NROM layout (32K PRG, 8K CHR-ROM,
# 8K WRAM at $6000 served by the cartridge) with $FFF0-$FFF9 reserved for
# the "FUJI" claim: that signature is what keeps the cartridge's mailbox
# alive after the image boots, so stamping it is part of linking rather than
# something each project is left to remember.
NES_CFG ?= $(MWD)/nes-fujinet.cfg
# With a linker map the stamp's RMW scan walks only the code segments, so a
# string that happens to look like an opcode cannot fail the build.
NES_MAP ?= $(EXECUTABLE:.nes=.map)

ifneq ($(IS_LIBRARY),1)
  LDFLAGS += -C $(NES_CFG) -m $(NES_MAP)

.DELETE_ON_ERROR:

$(PLATFORM)/executable-post::
	$(MWD)/nes-romstamp.py --stamp --map $(NES_MAP) $(EXECUTABLE)
endif

r2r:: $(BUILD_EXEC) $(BUILD_LIB) $(R2R_EXTRA_DEPS)
	make -f $(PLATFORM_MK) $(PLATFORM)/r2r-post

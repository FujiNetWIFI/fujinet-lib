EXEC_SUFFIX = .rom
LIBRARY = $(R2R_PD)/$(PRODUCT_BASE).$(PLATFORM).lib

MWD := $(realpath $(dir $(lastword $(MAKEFILE_LIST)))..)
include $(MWD)/common.mk
include $(MWD)/toolchains/z88dk.mk

# The 32K cart image contract (code and data below $F800, the "FUJI" claim
# at $FCFC) is cart layout, not bus; MekkoGX's coleco.mk carries it.
COLECO_FLAGS = +coleco
CFLAGS += $(COLECO_FLAGS)
LDFLAGS += $(COLECO_FLAGS)
ifneq ($(IS_LIBRARY),1)
  LDFLAGS += -create-app
endif

r2r:: $(BUILD_EXEC) $(BUILD_LIB) $(R2R_EXTRA_DEPS)
	make -f $(PLATFORM_MK) $(PLATFORM)/r2r-post

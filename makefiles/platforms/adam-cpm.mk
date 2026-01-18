EXECUTABLE = $(R2R_PD)/$(PRODUCT_BASE).com
DISK = $(R2R_PD)/$(PRODUCT_BASE).dsk
LIBRARY = $(R2R_PD)/$(PRODUCT_BASE).$(PLATFORM).lib
DISK_TOOL = cpmcp

MWD := $(realpath $(dir $(lastword $(MAKEFILE_LIST)))..)
include $(MWD)/common.mk
include $(MWD)/toolchains/z88dk.mk

ADAM_FLAGS = +cpm -subtype=adam
CFLAGS += $(ADAM_FLAGS)
LDFLAGS += $(ADAM_FLAGS)
LIBS += -lndos

r2r:: $(BUILD_DISK) $(BUILD_EXEC) $(BUILD_LIB) $(R2R_EXTRA_DEPS_$(PLATFORM_UC))
	make -f $(PLATFORM_MK) $(PLATFORM)/r2r-post

CPM_BLANK = blank160-bootable.dsk
CPM_DISK = $(CACHE_PLATFORM)/$(CPM_BLANK)

$(BUILD_DISK): $(BUILD_EXEC) $(CPM_DISK) $(DISK_EXTRA_DEPS_$(PLATFORM_UC)) | $(R2R_PD)
	$(call require,$(DISK_TOOL),$(DISK_TOOL_INFO))
	cp $(CPM_DISK) $@
	$(DISK_TOOL) -f coleco-adam $@ $< 0:
	make -f $(PLATFORM_MK) $(PLATFORM)/disk-post

CPM_DISK_URL = https://retrobattlestations.com/Coleco-Adam
$(CPM_DISK): | $(CACHE_PLATFORM)
	curl -L -o $@ $(CPM_DISK_URL)/$(CPM_BLANK)

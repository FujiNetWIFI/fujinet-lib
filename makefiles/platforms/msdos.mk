PRODUCT_BASE = $(PRODUCT_BASE)
EXECUTABLE = $(R2R_PD)/$(PRODUCT_BASE).exe
DISK = $(R2R_PD)/$(PRODUCT_BASE).img
LIBRARY = $(R2R_PD)/lib$(PRODUCT_BASE).$(PLATFORM).a
DISK_TOOL_CREATE = dd
DISK_TOOL_FORMAT = mformat
DISK_TOOL_COPY = mcopy
DISK_SIZE_KB = 360

MWD := $(realpath $(dir $(lastword $(MAKEFILE_LIST)))..)
include $(MWD)/common.mk
include $(MWD)/toolchains/ow2.mk

CFLAGS += -D__MSDOS__

r2r:: $(BUILD_DISK) $(BUILD_EXEC) $(BUILD_LIB) $(R2R_EXTRA_DEPS_$(PLATFORM_UC))
	make -f $(PLATFORM_MK) $(PLATFORM)/r2r-post

$(BUILD_DISK): $(BUILD_EXEC) | $(R2R_PD)
	$(RM) $@
	$(RM) -rf $(CACHE_PLATFORM)/disk
	$(MKDIR_P) $(CACHE_PLATFORM)/disk
	cp $< $(CACHE_PLATFORM)/disk
	$(DISK_TOOL_CREATE) if=/dev/zero of=$@ bs=1024 count=$(DISK_SIZE_KB)
	$(DISK_TOOL_FORMAT) -i $@ -f $(DISK_SIZE_KB)
	$(DISK_TOOL_COPY) -i $@ $(CACHE_PLATFORM)/disk/* "::/"
	@make -f $(PLATFORM_MK) $(PLATFORM)/disk-post

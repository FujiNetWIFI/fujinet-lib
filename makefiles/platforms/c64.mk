EXEC_SUFFIX = .prg
LIBRARY = $(R2R_PD)/$(PRODUCT_BASE).$(PLATFORM).lib
# d64, d71 or d81 also puts the programs on a disk image; empty builds only the .prg files
DISK_TYPE_C64 ?=
DISK = $(if $(strip $(DISK_TYPE_C64)),$(R2R_PD)/$(PRODUCT_BASE).$(DISK_TYPE_C64))
# A program written first on the disk, so a LOAD"*" boot runs it (a menu, say)
DISK_BOOT_C64 ?=
DISK_TOOL = cc1541
DISK_TOOL_INFO = https://bitbucket.org/JackAsser/cc1541

MWD := $(realpath $(dir $(lastword $(MAKEFILE_LIST)))..)
include $(MWD)/common.mk
include $(MWD)/toolchains/cc65.mk

# At most 16 characters; cc1541 turns lowercase ASCII into the C64's capitals
c64-name = $(shell echo $(basename $(notdir $(1))) | tr '[:upper:]' '[:lower:]' | cut -c1-16)

r2r:: $(BUILD_DISK) $(BUILD_EXEC) $(BUILD_LIB) $(R2R_EXTRA_DEPS)
	make -f $(PLATFORM_MK) $(PLATFORM)/r2r-post

ifneq ($(strip $(BUILD_DISK)),)
$(BUILD_DISK): $(DISK_BOOT_C64) $(DISK_EXECUTABLES) $(DISK_EXTRA_DEPS) $(DISK_EXTRA_FILES) | $(R2R_PD)
	$(RM) $@
	$(call require,$(DISK_TOOL),$(DISK_TOOL_INFO))
	$(DISK_TOOL) -n "$(call c64-name,$(PRODUCT_BASE))" \
	  $(foreach b,$(DISK_BOOT_C64),-f "$(call c64-name,$(b))" -w $(b)) \
	  $(foreach e,$(DISK_EXECUTABLES),-f "$(call c64-name,$(e))" -w $(e)) \
	  $(foreach f,$(DISK_EXTRA_FILES),-f "$(call c64-name,$(f))" -w $(f)) \
	  $@
	@make -f $(PLATFORM_MK) $(PLATFORM)/disk-post
endif

# Arguments:
# $1 == DISK_TOOL flags
# $2 == source file
# $3 == destination name
# $4 == disk image
define copy-to-disk
    $(DISK_TOOL) $1 -f "$3" -w $2 $4
endef

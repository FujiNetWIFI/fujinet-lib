# Generic Build script for CC65
#

# Customized for fujinet-libs

# Ensure WSL2 Ubuntu and other linuxes use bash by default instead of /bin/sh, which does not always like the shell commands.
SHELL := /usr/bin/env bash
ALL_TASKS =
DISK_TASKS =
OBJEXT = .o
ASMEXT = .s

-include ./makefiles/os.mk
-include ./makefiles/compiler.mk

SRCDIR := src
BUILD_DIR := build
OBJDIR := obj
DIST_DIR := dist

# This allows src to be nested withing sub-directories.
rwildcard=$(wildcard $(1)$(2))$(foreach d,$(wildcard $1*), $(call rwildcard,$d/,$2))

PLATFORM_SRC_DIR := $(CURRENT_PLATFORM)/$(SRCDIR)

PROGRAM_TGT := $(PROGRAM).$(CURRENT_TARGET)

SOURCES += $(call rwildcard,$(PLATFORM_SRC_DIR),*.c)
SOURCES += $(call rwildcard,$(PLATFORM_SRC_DIR),*$(ASMEXT))
SOURCES += $(call rwildcard,common/$(SRCDIR)/,*.c)
SOURCES += $(call rwildcard,common/$(SRCDIR)/,*$(ASMEXT))

################################################
# PLATFORM SPECIFIC PATHS/SOURCES

define ADD_SOURCES
SOURCES += $(call rwildcard,$(CURRENT_PLATFORM)/$(1),*.c)
SOURCES += $(call rwildcard,$(CURRENT_PLATFORM)/$(1),*$(ASMEXT))
endef

define ADD_SRC_INC_DIRS
SRC_INC_DIRS += $(sort $(dir $(wildcard $(CURRENT_PLATFORM)/$(1)/*)))
endef

$(foreach path,$(PLATFORM_SPECIFIC_PATHS),$(eval $(call ADD_SOURCES,$(path))))
$(foreach path,$(PLATFORM_SPECIFIC_PATHS),$(eval $(call ADD_SRC_INC_DIRS,$(path))))

################################################
# GENERATE OBJECT LIST FROM ALL SOURCES

# convert from src/your/long/path/foo.[c|s] to obj/<target>/your/long/path/foo.o
OBJ1 := $(SOURCES:.c=$(OBJEXT))
OBJECTS := $(OBJ1:$(ASMEXT)=$(OBJEXT))

# this needs to undergo all substitutions too, this is the list of files that will be added to the archive (library)
OBJECTS_ARC := $(OBJECTS)

# add any additional archive objects before doing all the substitutions
-include ./makefiles/objects-$(CURRENT_TARGET).mk

OBJECTS := $(OBJECTS:$(PLATFORM_SRC_DIR)/%=$(OBJDIR)/$(CURRENT_TARGET)/%)
OBJECTS := $(OBJECTS:common/$(SRCDIR)/%=$(OBJDIR)/$(CURRENT_TARGET)/common/%)

OBJECTS_ARC := $(OBJECTS_ARC:$(PLATFORM_SRC_DIR)/%=$(OBJDIR)/$(CURRENT_TARGET)/%)
OBJECTS_ARC := $(OBJECTS_ARC:common/$(SRCDIR)/%=$(OBJDIR)/$(CURRENT_TARGET)/common/%)

################################################
# PLATFORM SPECIFIC OBJECTS

define SUBSTITUTE_OBJECTS
$($(1):$(CURRENT_PLATFORM)/$(2)/%=$(OBJDIR)/$(CURRENT_TARGET)/%)
endef

$(foreach path,$(PLATFORM_SPECIFIC_PATHS),$(eval OBJECTS := $(call SUBSTITUTE_OBJECTS,OBJECTS,$(path))))
$(foreach path,$(PLATFORM_SPECIFIC_PATHS),$(eval OBJECTS_ARC := $(call SUBSTITUTE_OBJECTS,OBJECTS_ARC,$(path))))

# remove trailing and leading spaces.
SOURCES := $(strip $(SOURCES))
OBJECTS := $(strip $(OBJECTS))
OBJECTS_ARC := $(strip $(OBJECTS_ARC))

# ensure the paths are unique
# SRC_INC_DIRS := $(sort $(filter-out %/, $(SRC_INC_DIRS)))

# Ensure make recompiles parts it needs to if src files change
DEPENDS := $(OBJECTS:$(OBJEXT)=.d)

ifeq ($(CC),iix compile)
CFLAGS += \
	$(INCC_ARG)common/inc \
	$(INCC_ARG)$(PLATFORM_SRC_DIR)/include \
	$(INCC_ARG).
else ifeq ($(CC),zcc)
CFLAGS += \
	$(INCC_ARG)common/inc \
	$(INCC_ARG)$(PLATFORM_SRC_DIR)/include \
	$(INCC_ARG).

ASFLAGS += \
	$(INCS_ARG)common/inc \
	$(INCS_ARG)$(PLATFORM_SRC_DIR)/include \
	$(INCS_ARG).
else ifeq ($(CC),wcc)
CFLAGS += \
	$(INCC_ARG)common/inc \
	$(INCC_ARG)$(PLATFORM_SRC_DIR)/include \
	$(INCC_ARG).
else
ASFLAGS += \
	$(INCS_ARG) common/inc \
	$(INCS_ARG) $(PLATFORM_SRC_DIR)/include \
	$(INCS_ARG) .

CFLAGS += \
	$(INCC_ARG) common/inc \
	$(INCC_ARG) $(PLATFORM_SRC_DIR)/include \
	$(INCC_ARG) .
endif

FN_NW_HEADER = fujinet-network.h
FN_NW_INC = fujinet-network.inc
FN_FUJI_HEADER = fujinet-fuji.h
FN_FUJI_INC = fujinet-fuji.inc
FN_CLOCK_HEADER = fujinet-clock.h
FN_CLOCK_INC = fujinet-clock.inc
CHANGELOG = Changelog.md

# allow for additional flags etc
-include ./makefiles/common.mk
# global PLATFORM custom settings
-include ./makefiles/custom-$(CURRENT_PLATFORM).mk

# allow for application specific config
-include ./application.mk

.SUFFIXES:
.PHONY: all clean release $(PROGRAM_TGT) $(ALL_TASKS)

all: $(ALL_TASKS) $(PROGRAM_TGT)

-include $(DEPENDS)

$(OBJDIR):
	$(call MKDIR,$@)

$(TARGETOBJDIR):
	$(call MKDIR,$@)

$(BUILD_DIR):
	$(call MKDIR,$@)

$(DIST_DIR):
	$(call MKDIR,$@)

# Add the default paths for the PLATFORM and common
SRC_INC_DIRS += \
  $(sort $(dir $(wildcard $(PLATFORM_SRC_DIR)/*))) \
  $(sort $(dir $(wildcard common/$(SRCDIR)/*)))

vpath %.c $(SRC_INC_DIRS)
vpath %$(ASMEXT) $(SRC_INC_DIRS)

# $(info SOURCES: $(SOURCES))
# $(info OBJECTS: $(OBJECTS))
# $(info OBJECTS_ARC: $(OBJECTS_ARC))
# $(info SRC_INC_DIRS: $(SRC_INC_DIRS))

$(OBJDIR)/$(CURRENT_TARGET)/common/%$(OBJEXT): %.c | $(TARGETOBJDIR)
	@$(call MKDIR,$(dir $@))
ifeq ($(CC),cl65)
	$(CC) -t $(CURRENT_TARGET) -c --create-dep $(@:.o=.d) $(CFLAGS) --listing $(@:.o=.lst) -Ln $@.lbl -o $@ $<
else ifeq ($(CC),wcc)
	$(CC) $(CFLAGS) -fo=$@ $<
else ifeq ($(CC),iix compile)
	$(CC) $(CFLAGS) $< keep=$(subst .root,,$@)
else ifeq ($(CC),zcc)
	$(CC) -c $(CFLAGS) -o $@ $<
else
	$(CC) -c --deps $(CFLAGS) -o $@ $<
endif

$(OBJDIR)/$(CURRENT_TARGET)/%$(OBJEXT): %.c $(VERSION_FILE) | $(OBJDIR)
	@$(call MKDIR,$(dir $@))
ifeq ($(CC),cl65)
	$(CC) -t $(CURRENT_TARGET) -c --create-dep $(@:.o=.d) $(CFLAGS) --listing $(@:.o=.lst) -Ln $@.lbl -o $@ $<
else ifeq ($(CC),iix compile)
	$(CC) $(CFLAGS) $< keep=$(subst .root,,$@)
else ifeq ($(CC),zcc)
	$(CC) -c $(CFLAGS) -o $@ $<
else ifeq ($(CC),wcc)
	$(CC) $(CFLAGS) -fo=$@ $<
else
	$(CC) -c --deps $(CFLAGS) -o $@ $<
endif

## For now, no asm in common dirs... as it would be compiler specific
# $(OBJDIR)/$(CURRENT_TARGET)/common/%$(OBJEXT): %$(ASMEXT) | $(TARGETOBJDIR)
# 	@$(call MKDIR,$(dir $@))
# ifeq ($(CC),cl65)
# 	$(CC) -t $(CURRENT_TARGET) -c --create-dep $(@:.o=.d) $(ASFLAGS) -o $@ $<
# else
# endif

# below shell part is a hack to make foo.ROOT become foo.root, even though the output name is already foo.root, iix capitalizes the "ROOT" part in the filename, which breaks the linux version of linker
$(OBJDIR)/$(CURRENT_TARGET)/%$(OBJEXT): %$(ASMEXT) $(VERSION_FILE) | $(OBJDIR)
	@$(call MKDIR,$(dir $@))
ifeq ($(CC),cl65)
	$(CC) -t $(CURRENT_TARGET) -c --create-dep $(@:.o=.d) $(ASFLAGS) --listing $(@:.o=.lst) -Ln $@.lbl -o $@ $<
else ifeq ($(CC),iix compile)
	$(CC) $(ASFLAGS) $< keep=$(subst .root,,$@)
	@OUT_NAME="$@"; CAP_NAME=$${OUT_NAME//.root}.ROOT; if [ -f "$$CAP_NAME" ]; then mv $$CAP_NAME $@; fi
else ifeq ($(CC),zcc)
	$(CC) +$(CURRENT_TARGET) -c $(ASLAGS) -o $@ $<
else
	$(CC) -c --deps $(@:.o=.d) $(ASFLAGS) -o $@ $<
endif

$(BUILD_DIR)/$(PROGRAM_TGT): $(OBJECTS) | $(BUILD_DIR)
ifeq ($(CC),cl65)
	$(AR) a $@ $(OBJECTS_ARC)
else ifeq ($(CC),iix compile)

ifeq ($(detected_OS),$(filter $(detected_OS),MSYS MINGW))
	rm -rf $(OBJDIR)/flat; \
	FILE_LIST=$$(scripts/fix-makelib-path.sh $(OBJDIR)/$(CURRENT_TARGET) $(OBJDIR)/flat); \
	cd $(OBJDIR)/flat; \
	for f in $$(echo "$${FILE_LIST}" | tr ' ' '\n'); do $(AR) $(PROGRAM_TGT) $${f}; done; \
	mv $(PROGRAM_TGT) ../../$(BUILD_DIR)/
else
	rm -f $@
	$(AR) $@ $(addprefix +,$(OBJECTS_ARC))
endif

else ifeq ($(CC),zcc)
	$(AR) -x$@.lib $(OBJECTS)
	mv -v $@.lib $@
else ifeq ($(CC),wcc)
	$(AR) -q $@ $(OBJECTS)
else
	$(AR) $@ -a $(OBJECTS)
endif

$(PROGRAM_TGT): $(BUILD_DIR)/$(PROGRAM_TGT) | $(BUILD_DIR)

# Use "./" in front of all dirs being removed as a simple safety guard to
# ensure deleting from current dir, and not something like root "/".
clean:
	@for d in $(BUILD_DIR) $(OBJDIR) $(DIST_DIR); do \
      if [ -d "./$$d" ]; then \
	    echo "Removing $$d"; \
        rm -rf ./$$d; \
      fi; \
    done

release: all | $(BUILD_DIR) $(DIST_DIR)
	cp $(BUILD_DIR)/$(PROGRAM_TGT) $(DIST_DIR)/fujinet-$(CURRENT_TARGET)-$(VERSION_STRING).lib
	cp $(FN_NW_HEADER) dist/
	cp $(FN_NW_INC) dist/
	cp $(FN_FUJI_HEADER) dist/
	cp $(FN_FUJI_INC) dist/
	cp $(FN_CLOCK_HEADER) dist/
	cp $(FN_CLOCK_INC) dist/
	cp $(CHANGELOG) dist/
	cd dist && zip fujinet-lib-$(CURRENT_TARGET)-$(VERSION_STRING).zip $(CHANGELOG) fujinet-$(CURRENT_TARGET)-$(VERSION_STRING).lib *.h *.inc
	$(call RMFILES,dist/fujinet-$(CURRENT_TARGET)-*.lib)
	$(call RMFILES,dist/$(CHANGELOG))
	$(call RMFILES,dist/*.h)
	$(call RMFILES,dist/*.inc)

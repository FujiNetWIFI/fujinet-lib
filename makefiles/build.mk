# Generic Build script for CC65
#

# Customized for fujinet-libs

# Ensure WSL2 Ubuntu and other linuxes use bash by default instead of /bin/sh, which does not always like the shell commands.
SHELL := /usr/bin/env bash
ALL_TASKS =
DISK_TASKS =

-include ./makefiles/os.mk

CC := cl65

SRCDIR := src
BUILD_DIR := build
OBJDIR := obj
DIST_DIR := dist

# This allows src to be nested withing sub-directories.
rwildcard=$(wildcard $(1)$(2))$(foreach d,$(wildcard $1*), $(call rwildcard,$d/,$2))

PLATFORM_SRC_DIR := $(CURRENT_PLATFORM)/$(SRCDIR)

PROGRAM_TGT := $(PROGRAM).$(CURRENT_TARGET)

SOURCES := $(call rwildcard,$(PLATFORM_SRC_DIR),*.c)
SOURCES += $(call rwildcard,$(PLATFORM_SRC_DIR),*.s)
SOURCES += $(call rwildcard,common/$(SRCDIR)/,*.s)
SOURCES += $(call rwildcard,common/$(SRCDIR)/,*.c)

# remove trailing and leading spaces.
SOURCES := $(strip $(SOURCES))

# convert from src/your/long/path/foo.[c|s] to obj/<target>/your/long/path/foo.o
# we need the target because compiling for previous target does not pick up potential macro changes
OBJ1 := $(SOURCES:.c=.o)
OBJECTS := $(OBJ1:.s=.o)
OBJECTS := $(OBJECTS:$(PLATFORM_SRC_DIR)/%=$(OBJDIR)/$(CURRENT_TARGET)/%)
OBJECTS := $(OBJECTS:common/$(SRCDIR)/%=$(OBJDIR)/$(CURRENT_TARGET)/common/%)

# Ensure make recompiles parts it needs to if src files change
DEPENDS := $(OBJECTS:.o=.d)

ASFLAGS += \
	--asm-include-dir common/inc \
	--asm-include-dir $(PLATFORM_SRC_DIR)/include \
	--asm-include-dir .

CFLAGS += \
	--include-dir common/inc \
	--include-dir $(PLATFORM_SRC_DIR)/include \
	--include-dir .

FN_NW_HEADER = fujinet-network.h
FN_NW_INC = fujinet-network.inc
FN_FUJI_HEADER = fujinet-fuji.h
FN_FUJI_INC = fujinet-fuji.inc
CHANGELOG = Changelog.md

# allow for additional flags etc
-include ./makefiles/common.mk
-include ./makefiles/custom-$(CURRENT_PLATFORM).mk

# allow for application specific config
-include ./application.mk

define _listing_
  CFLAGS += --listing $$(@:.o=.lst)
  ASFLAGS += --listing $$(@:.o=.lst)
endef

define _mapfile_
  LDFLAGS += --mapfile $$@.map
endef

define _labelfile_
  LDFLAGS += -Ln $$@.lbl
endef

.SUFFIXES:
.PHONY: all clean release $(DISK_TASKS) $(BUILD_TASKS) $(PROGRAM_TGT) $(ALL_TASKS)

all: $(ALL_TASKS) $(PROGRAM_TGT)

STATEFILE := Makefile.options
-include $(DEPENDS)
-include $(STATEFILE)

ifeq ($(origin _OPTIONS_),file)
OPTIONS = $(_OPTIONS_)
$(eval $(OBJECTS): $(STATEFILE))
endif

# Transform the abstract OPTIONS to the actual cc65 options.
$(foreach o,$(subst $(COMMA),$(SPACE),$(OPTIONS)),$(eval $(_$o_)))

ifeq ($(BUILD_DIR),)
BUILD_DIR := build
endif

ifeq ($(OBJDIR),)
OBJDIR := obj
endif

ifeq ($(DIST_DIR),)
DIST_DIR := dist
endif

$(OBJDIR):
	$(call MKDIR,$@)

$(TARGETOBJDIR):
	$(call MKDIR,$@)

$(BUILD_DIR):
	$(call MKDIR,$@)

$(DIST_DIR):
	$(call MKDIR,$@)

SRC_INC_DIRS := \
  $(sort $(dir $(wildcard $(PLATFORM_SRC_DIR)/*))) \
  $(sort $(dir $(wildcard common/$(SRCDIR)/*)))


vpath %.c $(SRC_INC_DIRS)

$(OBJDIR)/$(CURRENT_TARGET)/common/%.o: %.c | $(TARGETOBJDIR)
	@$(call MKDIR,$(dir $@))
	$(CC) -t $(CURRENT_TARGET) -c --create-dep $(@:.o=.d) $(CFLAGS) -o $@ $<

$(OBJDIR)/$(CURRENT_TARGET)/%.o: %.c $(VERSION_FILE) | $(OBJDIR)
	@$(call MKDIR,$(dir $@))
	$(CC) -t $(CURRENT_TARGET) -c --create-dep $(@:.o=.d) $(CFLAGS) -o $@ $<

vpath %.s $(SRC_INC_DIRS)

$(OBJDIR)/$(CURRENT_TARGET)/common/%.o: %.s | $(TARGETOBJDIR)
	@$(call MKDIR,$(dir $@))
	$(CC) -t $(CURRENT_TARGET) -c --create-dep $(@:.o=.d) $(ASFLAGS) -o $@ $<

$(OBJDIR)/$(CURRENT_TARGET)/%.o: %.s $(VERSION_FILE) | $(OBJDIR)
	@$(call MKDIR,$(dir $@))
	$(CC) -t $(CURRENT_TARGET) -c --create-dep $(@:.o=.d) $(ASFLAGS) -o $@ $<


$(BUILD_DIR)/$(PROGRAM_TGT): $(OBJECTS) | $(BUILD_DIR)
	ar65 a $@ $(OBJECTS)

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
	cp $(CHANGELOG) dist/
	cd dist && zip fujinet-lib-$(CURRENT_TARGET)-$(VERSION_STRING).zip $(CHANGELOG) fujinet-$(CURRENT_TARGET)-$(VERSION_STRING).lib *.h *.inc
	$(call RMFILES,dist/fujinet-$(CURRENT_TARGET)-*.lib)
	$(call RMFILES,dist/$(CHANGELOG))
	$(call RMFILES,dist/*.h)
	$(call RMFILES,dist/*.inc)

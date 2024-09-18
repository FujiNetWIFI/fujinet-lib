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

# CC := cl65

SRCDIR := src
BUILD_DIR := build
OBJDIR := obj
DIST_DIR := dist

# This allows src to be nested withing sub-directories.
rwildcard=$(wildcard $(1)$(2))$(foreach d,$(wildcard $1*), $(call rwildcard,$d/,$2))

PLATFORM_SRC_DIR := $(CURRENT_PLATFORM)/$(SRCDIR)

PROGRAM_TGT := $(PROGRAM).$(CURRENT_TARGET)

SOURCES := $(call rwildcard,$(PLATFORM_SRC_DIR),*.c)
SOURCES += $(call rwildcard,$(PLATFORM_SRC_DIR),*$(ASMEXT))
SOURCES += $(call rwildcard,common/$(SRCDIR)/,*$(ASMEXT))
SOURCES += $(call rwildcard,common/$(SRCDIR)/,*.c)

# remove trailing and leading spaces.
SOURCES := $(strip $(SOURCES))

# convert from src/your/long/path/foo.[c|s] to obj/<target>/your/long/path/foo.o
# we need the target because compiling for previous target does not pick up potential macro changes
OBJ1 := $(SOURCES:.c=$(OBJEXT))
OBJECTS := $(OBJ1:$(ASMEXT)=$(OBJEXT))
OBJECTS_ORCA := $(OBJECTS) $(patsubst %.c,%.a,$(filter %.c,$(SOURCES)))
OBJECTS := $(OBJECTS:$(PLATFORM_SRC_DIR)/%=$(OBJDIR)/$(CURRENT_TARGET)/%)
OBJECTS := $(OBJECTS:common/$(SRCDIR)/%=$(OBJDIR)/$(CURRENT_TARGET)/common/%)
OBJECTS_ORCA := $(OBJECTS_ORCA:$(PLATFORM_SRC_DIR)/%=$(OBJDIR)/$(CURRENT_TARGET)/%)
OBJECTS_ORCA := $(OBJECTS_ORCA:common/$(SRCDIR)/%=$(OBJDIR)/$(CURRENT_TARGET)/common/%)

# Ensure make recompiles parts it needs to if src files change
DEPENDS := $(OBJECTS:$(OBJEXT)=.d)

ASFLAGS += \
	$(INCS_ARG) common/inc \
	$(INCS_ARG) $(PLATFORM_SRC_DIR)/include \
	$(INCS_ARG) .

ifeq ($(CC),iix compile)
CFLAGS += \
	$(INCC_ARG)common/inc \
	$(INCC_ARG)$(PLATFORM_SRC_DIR)/include \
	$(INCC_ARG).
else
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
-include ./makefiles/custom-$(CURRENT_PLATFORM).mk

# allow for application specific config
-include ./application.mk

.SUFFIXES:
.PHONY: all clean release $(PROGRAM_TGT) $(ALL_TASKS)

all: $(ALL_TASKS) $(PROGRAM_TGT)

-include $(DEPENDS)

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

$(OBJDIR)/$(CURRENT_TARGET)/common/%$(OBJEXT): %.c | $(TARGETOBJDIR)
	@$(call MKDIR,$(dir $@))
ifeq ($(CC),cl65)
	$(CC) -t $(CURRENT_TARGET) -c --create-dep $(@:.o=.d) $(CFLAGS) --listing $(@:.o=.lst) -Ln $@.lbl -o $@ $<
else ifeq ($(CC),iix compile)
	$(CC) $(CFLAGS) $< keep=$(subst .root,,$@)
else
	$(CC) -c --deps $(CFLAGS) -o $@ $<
endif

$(OBJDIR)/$(CURRENT_TARGET)/%$(OBJEXT): %.c $(VERSION_FILE) | $(OBJDIR)
	@$(call MKDIR,$(dir $@))
ifeq ($(CC),cl65)
	$(CC) -t $(CURRENT_TARGET) -c --create-dep $(@:.o=.d) $(CFLAGS) --listing $(@:.o=.lst) -Ln $@.lbl -o $@ $<
else ifeq ($(CC),iix compile)
	$(CC) $(CFLAGS) $< keep=$(subst .root,,$@)
else
	$(CC) -c --deps $(CFLAGS) -o $@ $<
endif

vpath %$(ASMEXT) $(SRC_INC_DIRS)

## For now, no asm in common dirs... as it would be compiler specific
# $(OBJDIR)/$(CURRENT_TARGET)/common/%$(OBJEXT): %$(ASMEXT) | $(TARGETOBJDIR)
# 	@$(call MKDIR,$(dir $@))
# ifeq ($(CC),cl65)
# 	$(CC) -t $(CURRENT_TARGET) -c --create-dep $(@:.o=.d) $(ASFLAGS) -o $@ $<
# else
# endif

$(OBJDIR)/$(CURRENT_TARGET)/%$(OBJEXT): %$(ASMEXT) $(VERSION_FILE) | $(OBJDIR)
	@$(call MKDIR,$(dir $@))
ifeq ($(CC),cl65)
	$(CC) -t $(CURRENT_TARGET) -c --create-dep $(@:.o=.d) $(ASFLAGS) --listing $(@:.o=.lst) -Ln $@.lbl -o $@ $<
else ifeq ($(CC),iix compile)
	$(CC) $(CFLAGS) $< keep=$(subst .root,,$@)
else
	$(CC) -c --deps $(@:.o=.d) $(ASFLAGS) -o $@ $<
endif

$(BUILD_DIR)/$(PROGRAM_TGT): $(OBJECTS) | $(BUILD_DIR)
ifeq ($(CC),cl65)
	$(AR) a $@ $(OBJECTS)
else ifeq ($(CC),iix compile)
	$(AR) $@ $(addprefix +,$(OBJECTS_ORCA))
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

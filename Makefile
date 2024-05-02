# Adapted from the generic cc65 makefile.
# Notible exceptions:
# - recursive dirs for src
# - final files go into build/ directory instead of root folder (e.g. lbl, com file etc)

TARGETS := atari apple2 commodore
PROGRAM := fujinet.lib
LIBS    :=
CONFIG  :=
CFLAGS  =
ASFLAGS =
LDFLAGS =
SRCDIR := src
OBJDIR := obj
EMUCMD :=
BUILD_DIR = build

CC65_TARGET_apple2     := apple2
CC65_TARGET_atari      := atari
CC65_TARGET_commodore  := c64

TARGETOBJDIR := $(OBJDIR)/$(TARGETS)

ifdef CC65_HOME
  CC := $(CC65_HOME)/bin/cl65
else
  CC := cl65
endif

ifeq ($(shell echo),)
  MKDIR = mkdir -p $1
  RMDIR = rmdir $1
  RMFILES = $(RM) $1
else
  MKDIR = mkdir $(subst /,\,$1)
  RMDIR = rmdir $(subst /,\,$1)
  RMFILES = $(if $1,del /f $(subst /,\,$1))
endif
COMMA := ,
SPACE := $(N/A) $(N/A)
define NEWLINE


endef
# Note: Do not remove any of the two empty lines above !

rwildcard=$(wildcard $(1)$(2))$(foreach d,$(wildcard $1*), $(call rwildcard,$d/,$2))

TARGETLIST := $(subst $(COMMA),$(SPACE),$(TARGETS))

ifeq ($(words $(TARGETLIST)),1)

# Strip potential variant suffix from the actual cc65 target.
CC65TARGET := $(firstword $(subst .,$(SPACE),$(TARGETLIST)))

# Set PROGRAM to something like 'myprog.c64'.
override PROGRAM := $(PROGRAM).$(TARGETLIST)

# Recursive files
SOURCES += $(call rwildcard,$(TARGETLIST)/$(SRCDIR)/,*.s)
SOURCES += $(call rwildcard,$(TARGETLIST)/$(SRCDIR)/,*.c)
SOURCES += $(call rwildcard,common/$(SRCDIR)/,*.s)
SOURCES += $(call rwildcard,common/$(SRCDIR)/,*.c)

# remove trailing and leading spaces.
SOURCES := $(strip $(SOURCES))

# convert from src/your/long/path/foo.[c|s] to obj/your/long/path/foo.o
OBJ1 := $(SOURCES:.c=.o)
OBJECTS := $(OBJ1:.s=.o)
# change from atari/src/ -> obj/atari/
OBJECTS := $(OBJECTS:$(TARGETLIST)/$(SRCDIR)/%=$(OBJDIR)/$(TARGETLIST)/%)
OBJECTS := $(OBJECTS:common/$(SRCDIR)/%=$(OBJDIR)/common/%)

DEPENDS := $(OBJECTS:.o=.d)

LIBS += $(wildcard $(TARGETLIST)/$(SRCDIR)/*.lib)

ASFLAGS += \
	--asm-include-dir common/inc \
	--asm-include-dir $(TARGETLIST)/$(SRCDIR)/fn_network/inc \
	--asm-include-dir $(TARGETLIST)/$(SRCDIR)/fn_fuji/inc \
	--asm-include-dir $(TARGETLIST)/$(SRCDIR)/bus/inc \
	--asm-include-dir .

CFLAGS += \
	--include-dir common/inc \
	--include-dir $(TARGETLIST)/$(SRCDIR)/fn_network/inc \
	--include-dir $(TARGETLIST)/$(SRCDIR)/fn_fuji/inc \
	--include-dir $(TARGETLIST)/$(SRCDIR)/bus/inc \
	--include-dir .

# Add -DBUILD_(TARGET) to all args for the current name. This allows some level of cross platform code
UPPER_TARGETLIST := $(shell echo $(TARGETLIST) | tr a-z A-Z)
CFLAGS += -DBUILD_$(UPPER_TARGETLIST)
ASFLAGS += -DBUILD_$(UPPER_TARGETLIST)
LDFLAGS += -DBUILD_$(UPPER_TARGETLIST)

CHANGELOG = Changelog.md

# single line with version number in semantic form (e.g. 2.1.3)
VERSION_FILE = version.txt
VERSION_STRING := $(file < $(VERSION_FILE))

# include files that are included in the ZIP dist/release target
FN_NW_HEADER = fujinet-network.h
FN_NW_INC = fujinet-network.inc
FN_FUJI_HEADER = fujinet-fuji.h
FN_FUJI_INC = fujinet-fuji.inc

.SUFFIXES:
.PHONY: all clean dist fujinet.lib.$(TARGETLIST)

all: fujinet.lib.$(TARGETLIST)

$(OBJDIR):
	$(call MKDIR,$@)

$(TARGETOBJDIR):
	$(call MKDIR,$@)

$(BUILD_DIR):
	$(call MKDIR,$@)

SRC_INC_DIRS := \
	$(sort $(dir $(wildcard $(TARGETLIST)/$(SRCDIR)/*))) \
	$(sort $(dir $(wildcard common/$(SRCDIR)/*)))

# $(info $$SOURCES = ${SOURCES})
# $(info $$OBJECTS = ${OBJECTS})
# $(info $$SRC_INC_DIRS = ${SRC_INC_DIRS})
# $(info $$ASFLAGS = ${ASFLAGS})
# $(info $$TARGETOBJDIR = ${TARGETOBJDIR})
# $(info $$TARGETLIST = ${TARGETLIST})

vpath %.c $(SRC_INC_DIRS)

obj/common/%.o: %.c | $(TARGETOBJDIR)
	@$(call MKDIR,$(dir $@))
	$(CC) -t $(CC65_TARGET_$(TARGETLIST)) -c --create-dep $(@:.o=.d) $(CFLAGS) -o $@ $<

$(TARGETOBJDIR)/%.o: %.c | $(TARGETOBJDIR)
	@$(call MKDIR,$(dir $@))
	$(CC) -t $(CC65_TARGET_$(TARGETLIST)) -c --create-dep $(@:.o=.d) $(CFLAGS) -o $@ $<

vpath %.s $(SRC_INC_DIRS)

obj/common/%.o: %.s | $(TARGETOBJDIR)
	@$(call MKDIR,$(dir $@))
	$(CC) -t $(CC65_TARGET_$(TARGETLIST)) -c --create-dep $(@:.o=.d) $(ASFLAGS) -o $@ $<

$(TARGETOBJDIR)/%.o: %.s | $(TARGETOBJDIR)
	@$(call MKDIR,$(dir $@))
	$(CC) -t $(CC65_TARGET_$(TARGETLIST)) -c --create-dep $(@:.o=.d) $(ASFLAGS) -o $@ $<

$(BUILD_DIR)/$(PROGRAM): $(OBJECTS) | $(BUILD_DIR)
	ar65 a $@ $(OBJECTS)

$(PROGRAM): $(BUILD_DIR)/$(PROGRAM) | $(BUILD_DIR)

clean:
	$(call RMFILES,$(OBJECTS))
	$(call RMFILES,$(DEPENDS))
	$(call RMFILES,$(REMOVES))
	$(call RMFILES,$(BUILD_DIR)/$(PROGRAM))

dist: $(PROGRAM)
	$(call MKDIR,dist/)
	$(call RMFILES,dist/fujinet-$(TARGETLIST)-*.lib)
	cp build/$(PROGRAM) dist/fujinet-$(TARGETLIST)-$(VERSION_STRING).lib
	cp $(FN_NW_HEADER) dist/
	cp $(FN_NW_INC) dist/
	cp $(FN_FUJI_HEADER) dist/
	cp $(FN_FUJI_INC) dist/
	cp $(CHANGELOG) dist/
	cd dist && zip fujinet-lib-$(TARGETLIST)-$(VERSION_STRING).zip $(CHANGELOG) fujinet-$(TARGETLIST)-$(VERSION_STRING).lib *.h *.inc
	$(call RMFILES,dist/fujinet-$(TARGETLIST)-*.lib)
	$(call RMFILES,dist/$(CHANGELOG))
	$(call RMFILES,dist/*.h)
	$(call RMFILES,dist/*.inc)

else # $(words $(TARGETLIST)),1

all:
	$(foreach t,$(TARGETLIST),$(MAKE) TARGETS=$t clean all dist$(NEWLINE))

endif # $(words $(TARGETLIST)),1


###################################################################
###  Place your additional targets in the additional Makefiles  ###
### in the same directory - their names have to end with ".mk"! ###
###################################################################
-include *.mk
###################################################################
# Platform Mapping, OS specifics, and Emulator settings
###################################################################

ifeq ($(CURRENT_TARGET),)
$(error Missing value for CURRENT_TARGET)
endif

CURRENT_PLATFORM_apple2 := apple2
CURRENT_PLATFORM_apple2enh := apple2
CURRENT_PLATFORM_apple2gs := apple2

CURRENT_PLATFORM_atari := atari
CURRENT_PLATFORM_atarixl := atari

CURRENT_PLATFORM_c64 := commodore
CURRENT_PLATFORM_c128 := commodore
CURRENT_PLATFORM_c16 := commodore
CURRENT_PLATFORM_pet := commodore
CURRENT_PLATFORM_plus4 := commodore
CURRENT_PLATFORM_cbm510 := commodore
CURRENT_PLATFORM_cbm610 := commodore
CURRENT_PLATFORM_vic20 := commodore

CURRENT_PLATFORM_coco := coco

CURRENT_PLATFORM_pmd85 := pmd85

CURRENT_PLATFORM_msdos := msdos

CURRENT_PLATFORM_adam := adam

CURRENT_PLATFORM = $(CURRENT_PLATFORM_$(CURRENT_TARGET))

# platform specific src paths (PSP)
# These allow us to add additional directories (each entry can have space separated list) to a TARGET
# Used for apple2/enh vs apple2gs, but other platforms if they had different compiled code could use it.

PSP_apple2 := apple2-6502
PSP_apple2enh := apple2-6502
PSP_apple2gs := apple2gs

PLATFORM_SPECIFIC_PATHS := $(PSP_$(CURRENT_TARGET))


ifeq '$(findstring ;,$(PATH))' ';'
    detected_OS := Windows
else
    detected_OS := $(shell uname 2>/dev/null || echo Unknown)
    detected_OS := $(patsubst CYGWIN%,Cygwin,$(detected_OS))
    detected_OS := $(patsubst MSYS%,MSYS,$(detected_OS))
    detected_OS := $(patsubst MINGW%,MSYS,$(detected_OS))
endif

# MSYS2 environment hack for extra slash (XS) needed in cmd line args
XS =
ifeq ($(detected_OS),$(filter $(detected_OS),MSYS MINGW))
	XS = /
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

PREEMUCMD :=
POSTEMUCMD :=

ifeq ($(EMUCMD),)
  EMUCMD = $($(CURRENT_TARGET)_EMUCMD)
endif

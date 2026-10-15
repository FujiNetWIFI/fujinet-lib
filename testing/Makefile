PRODUCTS = fnlib-test.lib
PRODUCTS += fujidev
PRODUCTS += appkey
PRODUCTS += clock
PRODUCTS += devlist
PRODUCTS += disk
PRODUCTS += netfile
PRODUCTS += net5plus
PRODUCTS += network
PRODUCTS += parserr
PRODUCTS += qrcode
PRODUCTS += html
PRODUCTS_DISK_NAME = fntests

PLATFORMS := $(shell awk '/^PLATFORMS[ \t]*\+=/{print $$3}' ../Makefile)

# You can run 'make <platform>' to build for a specific platform,
# or 'make <platform>/<target>' for a platform-specific target.
# Example shortcuts:
#   make coco        → build for coco
#   make apple2/disk → build the 'disk' target for apple2

# FUJINET_LIB can be
# - a version number such as 4.7.6
# - a directory which contains the libs for each platform
# - a zip file with an archived fujinet-lib
# - a URL to a git repo
# - empty which will use whatever is the latest
# - undefined, no fujinet-lib will be used
FUJINET_LIB = ..

# Define extra dirs ("combos") that expand with a platform.
# Format: platform+=combo1,combo2
PLATFORM_COMBOS = \
  c64+=commodore \
  atarixe+=atari \
  atarixl+=atari \
  msxrom+=msx \
  msxdos+=msx \
  adam_cpm+=adam \
  h89-cpm+=h89 \
  h89-hdos+=h89

fnlib-test.lib_SRC_DIRS = tests tests/%PLATFORM%

MAIN_SRC_DIRS = src src/%PLATFORM%
fujidev_SRC_DIRS = $(MAIN_SRC_DIRS)
appkey_SRC_DIRS = $(MAIN_SRC_DIRS)
clock_SRC_DIRS = $(MAIN_SRC_DIRS)
devlist_SRC_DIRS = $(MAIN_SRC_DIRS)
disk_SRC_DIRS = $(MAIN_SRC_DIRS)
netfile_SRC_DIRS = $(MAIN_SRC_DIRS)
net5plus_SRC_DIRS = $(MAIN_SRC_DIRS)
network_SRC_DIRS = $(MAIN_SRC_DIRS)
parserr_SRC_DIRS = $(MAIN_SRC_DIRS)
qrcode_SRC_DIRS = $(MAIN_SRC_DIRS)
html_SRC_DIRS = $(MAIN_SRC_DIRS)

fujidev_CFLAGS = -DFUJI_TESTS=1
network_CFLAGS = -DNETWORK_TESTS=1
net5plus_CFLAGS = -DNETWORK2_TESTS=1
html_CFLAGS = -DHTML_TESTS=1
parserr_CFLAGS = -DPARSE_ERROR_TESTS=1
clock_CFLAGS = -DCLOCK_TESTS=1
disk_CFLAGS = -DDISK_TESTS=1
qrcode_CFLAGS = -DQRCODE_TESTS=1
netfile_CFLAGS = -DFS_TESTS=1
appkey_CFLAGS = -DAPPKEY_TESTS=1
devlist_CFLAGS = -DDEVLIST_TESTS=1

include ../makefiles/toplevel-rules.mk

# Each test menu is chosen like FUJINET_LIB: empty for the latest release, a
# version such as 1.0.0, a git URL (#branch optional) or a directory.
# Undefined leaves that menu off. Pinned, because finding the latest release
# goes through the GitHub API, whose rate limit CI runs hit.
COCOMENU = 1.0.0
ADAMMENU = 1.0.0
MSDOSMENU = 1.0.0

menu-on = $(filter-out undefined,$(origin $1))
# A shell command printing the directory that holds menu $1's files
menu-dir = python3 testmenu.py $1 "$($2)" $(CACHE_DIR)/menus

msdos/disk-post::
	$(if $(call menu-on,MSDOSMENU),menu=$$($(call menu-dir,msdos-menu,MSDOSMENU)) && \
	  $(call copy-to-disk,,$$menu/dosmenu.exe,TESTMENU.EXE,$(DISK)))

# The test binaries do not fit the default single density ATR or a 140kb
# ProDOS volume.
DISK_TOOL_FLAGS_ATARI = -D -m -b MyPicoDos406
DISK_SIZE = 800kb

CFLAGS += -DBUILD_$(PLATFORM_UC) -DPLATFORM=$(DSTRING_OPEN)$(PLATFORM)$(DSTRING_CLOSE)
CFLAGS += -DTEST_PROGRAM=$(DSTRING_OPEN)$(PRODUCT)$(DSTRING_CLOSE)
LDFLAGS_EXTRA_COCO = --org=0E00 --limit=7C00
# Exit to EOS_START, which reboots from D1
LDFLAGS_EXTRA_ADAM = -pragma-define:CRT_ON_EXIT=0xFC30

TESTS_DIR = tests
TESTS_LIB = fnlib-test
TESTS_LIB_DIR = r2r/$(PLATFORM)

CFLAGS_EXTRA = $(call include-dir-flag,$(TESTS_DIR))

LIBS = $(call library-dir-flag,$(TESTS_LIB_DIR))
LIBS_EXTRA_ADAM = $(call library-flag,$(TESTS_LIB).$(PLATFORM).lib)
LIBS_EXTRA_ADAM_CPM = $(call library-flag,$(TESTS_LIB).$(PLATFORM).lib) \
	$(call library-flag,eos.lib) $(call library-flag,smartkeys.lib)
LIBS_EXTRA_APPLE2 = $(call library-flag,$(TESTS_LIB).$(PLATFORM).lib)
LIBS_EXTRA_ATARI = $(call library-flag,$(TESTS_LIB).$(PLATFORM).lib)
LIBS_EXTRA_C64 = $(call library-flag,$(TESTS_LIB).$(PLATFORM).lib)
LIBS_EXTRA_COCO = $(call library-flag,$(TESTS_LIB).$(PLATFORM))
LIBS_EXTRA_LYNX = $(call library-flag,$(TESTS_LIB).$(PLATFORM).lib)
LIBS_EXTRA_MSDOS = $(call library-flag,$(TESTS_LIB_DIR)/lib$(TESTS_LIB).$(PLATFORM).a)
LIBS_EXTRA_MODEL2 = $(call library-flag,$(TESTS_LIB).$(PLATFORM).lib)
LIBS_EXTRA_MSX = $(call library-flag,$(TESTS_LIB).$(PLATFORM).lib)
LIBS_EXTRA_PMD85 = $(call library-flag,$(TESTS_LIB).$(PLATFORM).lib)
LIBS_EXTRA_H89-CPM = $(call library-flag,$(TESTS_LIB).$(PLATFORM).lib)
LIBS_EXTRA_H89-HDOS = $(call library-flag,$(TESTS_LIB).$(PLATFORM).lib)

# If you need to add extra platform-specific steps, do it below:
#   coco/r2r:: coco/custom-step1
#   coco/r2r:: coco/custom-step2
# or
#   apple2/disk: apple2/custom-step1 apple2/custom-step2

# The tests fill a 157K disk, so the CoCo image is an HDB-DOS multi-disk
# file: COCO_DISKS disks back to back, selected with DRIVE n. coco.mk only
# formats it; disk-post puts the menu on each disk and the programs on them
# in order. Titles longer than decb's 9 characters go straight into track 17
# sector 17 (LSN 322 + 630n).
# AUTOEXEC.BAS has to stay type 0 ASCII or HDB-DOS DOAUTO will not run it.
COCO_DISKS = 2
COCO_PROGRAMS = $(foreach p,$(filter-out %.lib,$(PRODUCTS)),$(R2R_PD)/$(p)$(EXEC_SUFFIX))
ifeq ($(PLATFORM),coco)
DISK_EXECUTABLES =
endif

coco/disk-post::
	$(RM) $(DISK)
	$(DISK_TOOL) dskini -h$(COCO_DISKS) $(DISK)
	$(if $(call menu-on,COCOMENU),menu=$$($(call menu-dir,cocomenu,COCOMENU)) || exit 1;) \
	n=0; while [ $$n -lt $(COCO_DISKS) ]; do \
	  printf 'FujiNet Lib Tests #%d\000' $$((n + 1)) \
	    | dd of=$(DISK) bs=256 seek=$$((322 + 630 * n)) conv=notrunc 2>/dev/null; \
	  $(if $(call menu-on,COCOMENU),\
	  $(call copy-to-disk,-b -2,$$menu/cocomenu.bin,COCOMENU.BIN:$$n,$(DISK)) || exit 1; \
	  $(call copy-to-disk,-a -0 -l,$$menu/AUTOEXEC.BAS,AUTOEXEC.BAS:$$n,$(DISK)) || exit 1;) \
	  n=$$((n + 1)); \
	done
	n=0; for f in $(COCO_PROGRAMS); do \
	  name=$$(basename $$f | tr '[:lower:]' '[:upper:]'); \
	  need=$$(( ($$(wc -c < $$f) + 2303) / 2304 )); \
	  while [ "$$($(DISK_TOOL) free $(DISK),:$$n | sed -n 's/^Free granules: \([0-9]*\).*/\1/p')" -lt $$need ]; do \
	    n=$$((n + 1)); \
	    [ $$n -lt $(COCO_DISKS) ] || { echo "$$name does not fit on $(COCO_DISKS) disks" >&2; exit 1; }; \
	  done; \
	  $(call copy-to-disk,-b -2,$$f,$$name:$$n,$(DISK)) || exit 1; \
	done

adam/r2r-post::
	$(if $(call menu-on,ADAMMENU),menu=$$($(call menu-dir,adam-menu,ADAMMENU)) && \
	  cp $$menu/adam-menu.ddp $(R2R_PD)/)

# On PMD 85 the menu can be prepared using MRM Manager utility, which is its own repo.
ifeq ($(PLATFORM),pmd85)
PMD85_MENU_REPO ?= https://github.com/a8jan/mrmmngr
PMD85_MENU_DIR = $(CACHE_DIR)/pmd85/mrmmngr
PMD85_MRMMNGR  = $(PMD85_MENU_DIR)/mrmmngr
PMD85_MENU_MRM = $(PRODUCTS_DISK_NAME).mrm

$(PMD85_MENU_DIR):
	git clone --depth 1 $(PMD85_MENU_REPO) $@

$(PMD85_MRMMNGR): | $(PMD85_MENU_DIR)
	$(MAKE) -C $(PMD85_MENU_DIR)

# Expands to one recipe line (note the trailing blank line) adding file $1 to the MRM image
define pmd85-mrm-add
$(PMD85_MRMMNGR) $(R2R_PD)/$(PMD85_MENU_MRM) -fa $(R2R_PD)/$(1).bin -fn '$(1).?' -al

endef

pmd85/r2r-post:: $(PMD85_MRMMNGR)
	$(PMD85_MRMMNGR) $(R2R_PD)/$(PMD85_MENU_MRM) -ic -is 256
	$(foreach f,$(filter-out %.lib,$(PRODUCTS)),$(call pmd85-mrm-add,$(f)))
	$(PMD85_MRMMNGR) $(R2R_PD)/$(PMD85_MENU_MRM) -c
endif

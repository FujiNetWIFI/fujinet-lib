PRODUCTS = fnlib-test.lib flibtest fnettest fclktest fdsktest fqrctest fnfstest fapktest
PRODUCTS_DISK_NAME = fntests

PLATFORMS += adam
PLATFORMS += adam_cpm
PLATFORMS += apple2
PLATFORMS += atari
PLATFORMS += c64
PLATFORMS += coco
PLATFORMS += msdos
#PLATFORMS += msxrom

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
  adam_cpm+=adam

fnlib-test.lib_SRC_DIRS = tests tests/%PLATFORM%

flibtest_SRC_DIRS = src src/%PLATFORM%
fnettest_SRC_DIRS = src src/%PLATFORM%
fclktest_SRC_DIRS = src src/%PLATFORM%
fdsktest_SRC_DIRS = src src/%PLATFORM%
fqrctest_SRC_DIRS = src src/%PLATFORM%
fnfstest_SRC_DIRS = src src/%PLATFORM%
fapktest_SRC_DIRS = src src/%PLATFORM%

flibtest_CFLAGS = -DFUJI_TESTS=1
fnettest_CFLAGS = -DNETWORK_TESTS=1
fclktest_CFLAGS = -DCLOCK_TESTS=1
fdsktest_CFLAGS = -DDISK_TESTS=1
fqrctest_CFLAGS = -DQRCODE_TESTS=1
fnfstest_CFLAGS = -DFS_TESTS=1
fapktest_CFLAGS = -DAPPKEY_TESTS=1

include ../makefiles/toplevel-rules.mk

# Seven test binaries do not fit the default single density ATR or a 140kb
# ProDOS volume.
DISK_TOOL_FLAGS_ATARI = -D -m -b MyPicoDos406
DISK_SIZE = 800kb

CFLAGS += -DBUILD_$(PLATFORM_UC) -DPLATFORM=$(DSTRING_OPEN)$(PLATFORM)$(DSTRING_CLOSE)
LDFLAGS_EXTRA_COCO = --org=0E00 --limit=7C00

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
LIBS_EXTRA_MSDOS = $(call library-flag,$(TESTS_LIB_DIR)/lib$(TESTS_LIB).$(PLATFORM).a)

# If you need to add extra platform-specific steps, do it below:
#   coco/r2r:: coco/custom-step1
#   coco/r2r:: coco/custom-step2
# or
#   apple2/disk: apple2/custom-step1 apple2/custom-step2

# cocomenu is its own repo and keeps no built files in it, so clone and build
COCOMENU_REPO ?= https://github.com/RichStephens/cocomenu.git
COCOMENU_DIR = $(CACHE_DIR)/coco/cocomenu
COCOMENU_BIN = $(COCOMENU_DIR)/cocomenu.bin
COCOMENU_BAS = $(COCOMENU_DIR)/AUTOEXEC.BAS

$(COCOMENU_DIR):
	git clone --depth 1 $(COCOMENU_REPO) $@

$(COCOMENU_BIN) $(COCOMENU_BAS): | $(COCOMENU_DIR)
	$(MAKE) -C $(COCOMENU_DIR) $(@F)

# AUTOEXEC.BAS has to stay type 0 ASCII or HDB-DOS DOAUTO will not run it.
# NO_COCO_MENU=1 builds a plain disk with just the test binaries.
ifndef NO_COCO_MENU
coco/disk-post:: $(COCOMENU_BIN) $(COCOMENU_BAS)
	$(call copy-to-disk,-b -2,$(COCOMENU_BIN),COCOMENU.BIN,$(DISK))
	$(call copy-to-disk,-a -0 -l,$(COCOMENU_BAS),AUTOEXEC.BAS,$(DISK))
endif

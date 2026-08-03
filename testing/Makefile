PRODUCTS = fnlib-test.lib flibtest fnettest fclktest fdsktest fqrctest
PRODUCTS_DISK_NAME = fntests

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

flibtest_CFLAGS = -DFUJI_TESTS=1
fnettest_CFLAGS = -DNETWORK_TESTS=1
fclktest_CFLAGS = -DCLOCK_TESTS=1
fdsktest_CFLAGS = -DDISK_TESTS=1
fqrctest_CFLAGS = -DQRCODE_TESTS=1

include ../makefiles/toplevel-rules.mk

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

PRODUCT = fujinet.lib
PLATFORMS += adam
PLATFORMS += apple2
PLATFORMS += atari
PLATFORMS += c64
PLATFORMS += coco
PLATFORMS += lynx
PLATFORMS += model2
PLATFORMS += msdos
PLATFORMS += msx

# You can run 'make <platform>' to build for a specific platform,
# or 'make <platform>/<target>' for a platform-specific target.
# Example shortcuts:
#   make coco        → build for coco
#   make apple2/disk → build the 'disk' target for apple2

# SRC_DIRS may use the literal %PLATFORM% token.
# It expands to the chosen PLATFORM plus any of its combos.
SRC_DIRS = common bus/%PLATFORM%
INCLUDE_DIRS = include
CFLAGS = -DBUILD_$(PLATFORM_UC)

include makefiles/toplevel-rules.mk

# If you need to add extra platform-specific steps, do it below:
#   coco/r2r:: coco/custom-step1
#   coco/r2r:: coco/custom-step2
# or
#   apple2/disk: apple2/custom-step1 apple2/custom-step2

RELEASE_VERSION ?= $(shell awk '/FNLIB_VERSION_FULL/{ gsub(/"/, "", $$3); print $$3 }' include/fujinet-version.h)

.PHONY: testing

testing:
	$(MAKE) -C testing clean all

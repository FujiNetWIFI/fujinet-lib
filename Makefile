PRODUCT = fujinet.lib
PLATFORMS += adam
PLATFORMS += apple2
PLATFORMS += atari
PLATFORMS += c64
PLATFORMS += coco
PLATFORMS += h89-cpm
PLATFORMS += h89-hdos
PLATFORMS += lynx
PLATFORMS += model2
PLATFORMS += msdos
PLATFORMS += msx
PLATFORMS += pmd85

# You can run 'make <platform>' to build for a specific platform,
# or 'make <platform>/<target>' for a platform-specific target.
# Example shortcuts:
#   make coco        → build for coco
#   make apple2/disk → build the 'disk' target for apple2

# SRC_DIRS may use the literal %PLATFORM% token.
# It expands to the chosen PLATFORM plus any of its combos.
SRC_DIRS = common bus/%PLATFORM% bus/%PLATFORM%/unapi
INCLUDE_DIRS = include
CFLAGS = -DBUILD_$(PLATFORM_UC)

# Define extra dirs ("combos") that expand with a platform.
# Format: platform+=combo1,combo2
PLATFORM_COMBOS = \
  h89-cpm+=h89 \
  h89-hdos+=h89

include makefiles/toplevel-rules.mk

# If you need to add extra platform-specific steps, do it below:
#   coco/r2r:: coco/custom-step1
#   coco/r2r:: coco/custom-step2
# or
#   apple2/disk: apple2/custom-step1 apple2/custom-step2

RELEASE_VERSION ?= $(shell awk '/FNLIB_VERSION_FULL/{ gsub(/"/, "", $$3); print $$3 }' include/fujinet-version.h)

# Apps expect the library in a release zip to be named fujinet-<platform>-<version>.lib
ifneq ($(PLATFORM),)
RELEASE_LIB = $(RELEASE_DIR)/fujinet-$(PLATFORM)-$(patsubst v%,%,$(RELEASE_VERSION)).lib
release:: $(PLATFORM)/release
	cp $(LIBRARY) $(RELEASE_LIB)
	-zip -q -d $(RELEASE_ZIP) $(notdir $(LIBRARY))
	zip -j $(RELEASE_ZIP) $(RELEASE_LIB)
	rm $(RELEASE_LIB)
endif

.PHONY: testing

testing:
	$(MAKE) -C testing clean all

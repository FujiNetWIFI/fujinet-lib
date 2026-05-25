TESTS_DIR = tests
TESTS_LIB = fnlib-test
TESTS_LIB_DIR = r2r/$(PLATFORM)

CFLAGS += -DBUILD_$(PLATFORM_UC)
CFLAGS_EXTRA = $(call include-dir-flag,$(TESTS_DIR))

LIBS = $(call library-dir-flag,$(TESTS_LIB_DIR))
LIBS_EXTRA_APPLE2 = $(call library-flag,$(TESTS_LIB).$(PLATFORM).lib)
LIBS_EXTRA_ATARI = $(call library-flag,$(TESTS_LIB).$(PLATFORM).lib)
LIBS_EXTRA_C64 = $(call library-flag,$(TESTS_LIB).$(PLATFORM).lib)
LIBS_EXTRA_COCO = $(call library-flag,$(TESTS_LIB).$(PLATFORM))
LIBS_EXTRA_MSDOS = $(call library-flag,$(TESTS_LIB_DIR)/lib$(TESTS_LIB).$(PLATFORM).a)

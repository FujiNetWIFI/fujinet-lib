PRODUCT = fdsktest

# SRC_DIRS may use the literal %PLATFORM% token.
# It expands to the chosen PLATFORM plus any of its combos.
SRC_DIRS = src src/%PLATFORM%
MEKKO_CONFIG = fdsk.mk
CFLAGS += -DDISK_TESTS=1

include linkbin.mk
include shared.mk

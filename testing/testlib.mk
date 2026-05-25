PRODUCT = fnlib-test.lib

# SRC_DIRS may use the literal %PLATFORM% token.
# It expands to the chosen PLATFORM plus any of its combos.
SRC_DIRS = tests tests/%PLATFORM%
MEKKO_CONFIG = testlib.mk

include shared.mk

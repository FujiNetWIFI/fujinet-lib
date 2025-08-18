CC := cmoc
AS := lwasm
AR := lwar
CFLAGS=-Iinclude -Ibus/coco
AFLAGS=
SHELL=/bin/bash -o pipefail

define compile
  $(CC) -c $(CFLAGS) --deps=$(<:.c=.$(PLATFORM).d) -o $@ $< 2>&1 | sed -e 's/'$$'\033''[[][0-9][0-9]*m//g'
endef

define assemble
  $(CC) -c $(AFLAGS) -o $@ $< 2>&1 | sed -e 's/'$$'\033''[[][0-9][0-9]*m//g'
endef

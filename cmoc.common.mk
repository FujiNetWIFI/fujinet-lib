CC := cmoc
AS := lwasm
AR := lwar
CFLAGS=-Iinclude -Ibus/coco
AFLAGS=

define compile
  $(CC) -c $(CFLAGS) -o $@ $<
endef

define assemble
  $(CC) -c $(AFLAGS) -o $@ $<
endef

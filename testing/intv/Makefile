# Build the Intellivision test cart. IntyBASIC and as1600 are not part of
# the fujinet-lib platform build; run this by hand.
#
# Needs IntyBASIC 1.5.0 or later (OPTION MAP) and as1600. LIBDIR is the
# IntyBASIC checkout holding intybasic_prologue.asm / intybasic_epilogue.asm
# for that version, e.g.
#   make INTYBASIC=/path/to/intybasic LIBDIR=$HOME/Workspace/IntyBASIC/
#
#   make                 menu cart       -> $(BUILD_DIR)/intv/fnlib-test.bin + .cfg
#   make PROG=appkey     auto-run cart   -> $(BUILD_DIR)/intv/fnlib-test-appkey.bin + .cfg
# BUILD_DIR defaults to the repo's build/; override it to build elsewhere.
#
BUILD_DIR ?= ../../build
BUILD     := $(BUILD_DIR)/intv
INTYBASIC ?= intybasic
AS1600    ?= as1600
LIBDIR    ?= $(HOME)/Workspace/IntyBASIC/

# Order is the menu's: key 1..9, 0, then page 2.
PROGRAMS = fujidev appkey clock devlist disk netfile net5plus network parserr qrcode html clipbord netproto wifi

TESTS = $(wildcard tests/*.t.bas)

ifdef PROG
IDX  = $(shell echo $(PROGRAMS) | tr ' ' '\n' | grep -n -x $(PROG) | cut -d: -f1)
NAME = fnlib-test-$(PROG)
AUTORUN = $(IDX)
else
NAME = fnlib-test
AUTORUN = 0
endif

.DELETE_ON_ERROR:
.PHONY: all clean

all: $(BUILD)/$(NAME).bin

$(BUILD)/config.bas: FORCE
	@mkdir -p $(BUILD)
	@echo "    CONST AUTORUN = $(AUTORUN)" > $@.new
	@cmp -s $@.new $@ && rm $@.new || mv $@.new $@

$(BUILD)/names.bas: $(TESTS) tools/genchecks.py
	@mkdir -p $(BUILD)
	python3 tools/genchecks.py $(BUILD) $(TESTS)

$(BUILD)/$(NAME).asm: main.bas harness.bas fnlib.bas $(BUILD)/names.bas $(BUILD)/config.bas
	$(INTYBASIC) main.bas $@ $(LIBDIR)

$(BUILD)/$(NAME).bin: $(BUILD)/$(NAME).asm
	$(AS1600) -o $@ -l $(BUILD)/$(NAME).lst -s $(BUILD)/$(NAME).sym $<

FORCE:

clean:
	rm -rf $(BUILD)

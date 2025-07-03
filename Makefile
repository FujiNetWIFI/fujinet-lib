PLATFORMS := apple2 c64 coco atari rs232
TARGETS := fujinet.apple2.lib libfujinet.coco.a
#ALL_TARGETS := fujinet.atari.lib fujinet.apple2.lib fujinet.c64.lib libfujinet.coco.a
ALL_TARGETS = $(addprefix $(BUILDDIR)/, $(TARGETS))

-include defs.mk

all: $(ALL_TARGETS)

$(BUILDDIR)/fujinet.apple2.lib: $(CFILES) $(HFILES) $(AFILES) $(CFILES_A2) $(BUILDDIR)
	$(MAKE) -f apple2.mk

$(BUILDDIR)/libfujinet.coco.a: $(CFILES) $(HFILES) $(AFILES) $(BUILDDIR) $(CFILES_COCO)
	$(MAKE) -f coco.mk

clean:
	rm -rf *.o *.lst *.map *.bin *.lib *_obj $(BUILDDIR) $(ALL_TARGETS)

-include post.mk

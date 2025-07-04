PLATFORMS := apple2 c64 coco atari rs232
TARGETS := fujinet.apple2.lib libfujinet.coco.a fujinet.c64.lib
#ALL_TARGETS := fujinet.atari.lib fujinet.apple2.lib fujinet.c64.lib libfujinet.coco.a
ALL_TARGETS = $(addprefix $(BUILDDIR)/, $(TARGETS))

-include defs.mk

all: $(ALL_TARGETS)

$(BUILDDIR)/fujinet.apple2.lib: $(CFILES) $(HFILES) $(AFILES) \
				$(CFILES_A2) $(AFILES_A2) \
				$(BUILDDIR)
	$(MAKE) -f apple2.mk

$(BUILDDIR)/libfujinet.coco.a: $(CFILES) $(HFILES) $(AFILES) \
			       $(CFILES_COCO) \
			       $(BUILDDIR)
	$(MAKE) -f coco.mk

$(BUILDDIR)/fujinet.c64.lib: $(CFILES) $(HFILES) $(AFILES) \
			     $(CFILES_C64) $(AFILES_C64) \
			     $(BUILDDIR)
	$(MAKE) -f c64.mk

clean:
	rm -rf *.o *.lst *.map *.bin *.lib *_obj $(BUILDDIR) $(ALL_TARGETS)

-include post.mk

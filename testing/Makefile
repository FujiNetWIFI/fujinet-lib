SUBPROJECTS = testlib fuji network clock fdsk

.PHONY: all $(SUBPROJECTS)

all: $(SUBPROJECTS)

$(SUBPROJECTS):
	$(MAKE) -f $@.mk $(PLATFORM)

atari apple2 coco msdos c64 adam:
	$(MAKE) PLATFORM=$@ all

clean:
	$(MAKE) -f shared.mk $@

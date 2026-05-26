SUBPROJECTS = testlib fuji network clock

.PHONY: all $(SUBPROJECTS)

all: $(SUBPROJECTS)

$(SUBPROJECTS):
	$(MAKE) -f $@.mk $(PLATFORM)

atari apple2 coco msdos:
	$(MAKE) PLATFORM=$@ all

clean:
	$(MAKE) -f shared.mk $@

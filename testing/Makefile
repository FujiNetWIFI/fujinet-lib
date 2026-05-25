SUBPROJECTS = testlib fuji network clock

all:
	for p in $(SUBPROJECTS); do \
	  $(MAKE) -f $$p.mk $(PLATFORM) ; \
	done

atari apple2 coco:
	$(MAKE) PLATFORM=$@ all

clean:
	$(MAKE) -f shared.mk $@

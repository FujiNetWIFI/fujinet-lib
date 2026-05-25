SUBPROJECTS = testlib fuji network clock

all:
	for p in $(SUBPROJECTS); do \
	  $(MAKE) -f $$p.mk; \
	done

clean:
	$(MAKE) -f shared.mk $@

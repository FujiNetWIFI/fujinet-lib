PLATFORM=atari
-include defs.mk

fujinet.$(PLATFORM).lib: $(OBJS) $(OBJS_A8)
	$(AR) a $@ $(OBJS)

-include cc65.common.mk
-include post.mk

PLATFORM=c64
-include defs.mk

fujinet.$(PLATFORM).lib: $(OBJS) $(OBJS_COCO)
	$(AR) a $@ $(OBJS)

-include cc65.common.mk
-include post.mk

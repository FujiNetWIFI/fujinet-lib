PLATFORM=apple2
TARGET=$(BUILDDIR)/fujinet.$(PLATFORM).lib
-include defs.mk

all: $(FNLIB) test.$(PLATFORM)

$(TARGET): $(OBJS) $(OBJS_A2)
	$(AR) a $@ $(OBJS) $(OBJS_A2)

define single-to-double
  unsingle $< && mv $<.ad $@ && mv .AppleDouble/$<.ad .AppleDouble/$@
endef

FNLIBS=-L $(BUILDDIR) fujinet.$(PLATFORM).lib

test.$(PLATFORM): test.bin
	$(single-to-double)
test.bin: test.c $(TARGET)
	$(CC) -t $(PLATFORM) $(CFLAGS) $(LDFLAGS) $< -o $@ $(LIBS) $(FNLIBS)

-include cc65.common.mk
-include post.mk

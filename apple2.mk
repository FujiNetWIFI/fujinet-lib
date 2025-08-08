PLATFORM=apple2
TARGET=$(BUILDDIR)/fujinet.$(PLATFORM).lib
# Converts AppleSingle (cc65 output) to AppleDouble (netatalk share)
UNSINGLE=unsingle
-include defs.mk

vpath %.c bus/$(PLATFORM)

all: $(FNLIB) test.$(PLATFORM)

$(TARGET): $(OBJS) $(OBJS_A2) $(BUILDDIR)
	$(AR) a $@ $(OBJS) $(OBJS_A2)

define single-to-double
  $(UNSINGLE) $< && mv $<.ad $@ && mv .AppleDouble/$<.ad .AppleDouble/$@
endef

test.$(PLATFORM): test.a2s
	if command -v $(UNSINGLE) > /dev/null 2>&1 ; then \
	  $(single-to-double) ; \
	else \
	  cp $< $@ ; \
	fi

test.a2s: test.c $(TARGET)
	$(CC) -t $(PLATFORM) $(CFLAGS) $(LDFLAGS) $< -o $@ $(LIBS) $(TARGET)

-include cc65.common.mk
-include post.mk

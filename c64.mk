PLATFORM = c64
TARGET=$(BUILDDIR)/fujinet.$(PLATFORM).lib
-include defs.mk

vpath %.c bus/$(PLATFORM)

all: $(TARGET) test.$(PLATFORM)

$(TARGET): $(OBJS) $(OBJS_C64) $(BUILDDIR)
	$(AR) a $@ $(OBJS) $(OBJS_C64)

test.$(PLATFORM): $(OBJDIR)/test.o $(TARGET)
	$(CC) -t $(PLATFORM) -o $@ $(LDFLAGS) $^

-include cc65.common.mk
-include post.mk

PLATFORM = atari
TARGET=$(BUILDDIR)/fujinet.$(PLATFORM).lib
-include defs.mk

vpath %.c bus/$(PLATFORM)

all: $(TARGET) test.$(PLATFORM)

$(TARGET): $(OBJS) $(OBJS_ATARI) $(BUILDDIR)
	$(AR) a $@ $(OBJS) $(OBJS_ATARI)

test.$(PLATFORM): $(OBJDIR)/test.o $(TARGET)
	$(CC) -t $(PLATFORM) -o $@ $(LDFLAGS) $^

-include cc65.common.mk
-include post.mk

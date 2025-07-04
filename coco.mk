PLATFORM = coco
TARGET = $(BUILDDIR)/libfujinet.$(PLATFORM).a
-include defs.mk

vpath %.c bus/$(PLATFORM)

all: $(TARGET) test.$(PLATFORM)

$(TARGET): $(OBJS) $(OBJS_COCO) $(BUILDDIR)
	$(AR) -a -r $@ $^

test.$(PLATFORM): $(OBJDIR)/test.o $(TARGET)
	$(CC) -o $@ $(LDFLAGS) $^

-include cmoc.common.mk
-include post.mk

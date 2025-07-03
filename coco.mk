TARGET = $(BUILDDIR)/libfujinet.$(PLATFORM).a
PLATFORM = coco
-include defs.mk

vpath %.c bus/coco

$(info OBJS_COCO: $(OBJS_COCO))

all: $(TARGET) test.$(PLATFORM)

$(TARGET): $(OBJS) $(OBJS_COCO) $(BUILDDIR)
	$(AR) -a $@ $^

test.$(PLATFORM): $(OBJDIR)/test.o
	$(CC) -o $@ $(LDFLAGS) $^ $(TARGET)

-include cmoc.common.mk
-include post.mk

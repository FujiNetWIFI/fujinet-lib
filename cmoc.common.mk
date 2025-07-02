CC=cmoc
AS=lwasm
FUJINET_LIB_DIR=$(HOME)/source/fujinet/fujinet-lib
CFLAGS=-I$(FUJINET_LIB_DIR) -I$(FUJINET_LIB_DIR)/coco/src/include
AFLAGS=
LIBS=-L $(FUJINET_LIB_DIR)/build -lfujinet.$(PLATFORM)

define link-bin
  $(CC) -o $@ $(LDFLAGS) $^ $(LIBS)
endef

$(OBJDIR)/%.o: %.c | $(OBJDIR)
	$(CC) -c $(CFLAGS) -o $@ $<

$(OBJDIR)/%.o: %.s | $(OBJDIR)
	$(CC) -c $(AFLAGS) -o $@ $<

$(OBJDIR):
	mkdir -p $@

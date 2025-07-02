CC := cl65
AR := ar65
CFLAGS = -O --cpu 6502 -I include
AFLAGS = --cpu 6502 --asm-include-dir include/cc65

$(OBJDIR)/%.o: %.c | $(OBJDIR)
	$(CC) -l $(basename $@).lst -c $(CFLAGS) -t $(PLATFORM) -o $@ $<

$(OBJDIR)/%.o: %.s | $(OBJDIR)
	$(CC) -l $(basename $@).lst -c $(AFLAGS) -t $(PLATFORM) -o $@ $<

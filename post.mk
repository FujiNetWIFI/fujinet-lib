$(BUILDDIR):
	mkdir -p $@

$(OBJDIR):
	mkdir -p $@

$(OBJDIR)/%.o: %.c | $(OBJDIR)
	$(compile)
$(OBJDIR)/%.o: common/%.c | $(OBJDIR)
	$(compile)
$(OBJDIR)/%.o: bus/$(PLATFORM)/%.c | $(OBJDIR)
	$(compile)

$(OBJDIR)/%.o: common/%.s | $(OBJDIR)
	$(assemble)
$(OBJDIR)/%.o: bus/$(PLATFORM)/%.s | $(OBJDIR)
	$(assemble)

CC        := iix compile
AR        := iix makelib
CFLAGS    := -I -P +F cc=-p\"apple2:src:include:orca.h\" cc=-d__APPLE2__
OBJEXT    := .root

INCC_ARG  := cc=-i
INCS_ARG  := 

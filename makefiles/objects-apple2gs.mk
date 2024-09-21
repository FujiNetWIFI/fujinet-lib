# target (not platform) changes to OBJECTS/OBJECTS_ARC list

# this adds all the c files as a files to the archive, as the compiler creates .root and .a files for them.
OBJECTS_ARC += $(patsubst %.c,%.a,$(filter %.c,$(SOURCES)))

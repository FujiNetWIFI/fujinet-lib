# sets the compiler and C args according to the target

ifeq ($(CURRENT_TARGET),coco)

-include makefiles/compiler-cmoc.mk

else ifeq ($(CURRENT_TARGET),apple2gs)

-include makefiles/compiler-orca.mk

else

-include makefiles/compiler-cc65.mk

endif
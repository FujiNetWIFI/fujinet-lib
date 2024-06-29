# sets the compiler and C args according to the target

ifeq ($(CURRENT_TARGET),coco)

-include makefiles/compiler-cmoc.mk

else

-include makefiles/compiler-cc65.mk

endif
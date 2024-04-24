TARGET_LIB ?= fn_network

BUILD_DIR ?= ./build
SRC_DIRS ?= ./src

CC=cmoc
LWAR=lwar

SRCS := $(shell find $(SRC_DIRS) -name *.c)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

CFLAGS=

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

$(BUILD_DIR)/$(TARGET_LIB): $(OBJS)
	$(LWAR) --create $(TARGET_LIB).a $(OBJS)

# c source
$(BUILD_DIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(CFLAGS) -o $@ -c $<

.PHONY: clean

clean:
	$(RM) -rf $(TARGET_LIB)* build

-include $(DEPS)

MKDIR_P ?= mkdir -p

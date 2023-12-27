.PHONY: all

TARGET := load_config
TARGET_LINUX := $(TARGET)
TARGET_WIN := $(TARGET).exe

ifeq ($(CONFIG_OS_LINUX), y)
TARGET = $(TARGET_LINUX)
endif
ifeq ($(CONFIG_OS_WIN), y)
TARGET = $(TARGET_WIN)
endif

all: $(TARGET)

%.o: %.c
	@$(CC) $(CFLAGS) -c -o $@ $<

ifeq ($(CONFIG_OS_LINUX), y)
$(TARGET): %: %.o
	@$(CC) -o $@ $< $(ROOT_DIR)/$(TARGET_STATIC_LIB) $(LDFLAGS)
endif

ifeq ($(CONFIG_OS_WIN), y)
$(TARGET): %.exe: %.o
	@$(CC) -o $@ $< $(ROOT_DIR)/$(TARGET_STATIC_LIB) $(LDFLAGS)
endif

clean:
ifneq ($(wildcard *.o $(TARGET)),)
	@echo "Cleaning load_config..."
	@rm -r *.o $(TARGET)
endif

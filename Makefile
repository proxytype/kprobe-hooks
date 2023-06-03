OUTPUT_DIR := bin

SRC_DIR := src

obj-m := $(OUTPUT_DIR)/drv1.o

SRCS := $(wildcard $(SRC_DIR)/*.*)

all: $(OUTPUT_DIR)/drv1.o
	$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

# Clean rule
clean:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

$(OUTPUT_DIR)/drv1.o: $(SRCS) | $(OUTPUT_DIR)
	$(foreach src,$(SRCS),cp $(src) $(OUTPUT_DIR)/;)

$(OUTPUT_DIR):
	mkdir -p $(OUTPUT_DIR)

.PHONY: all clean

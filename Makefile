# SPDX-License-Identifier: GPL-2.0-only
LLVM     := /opt/homebrew/opt/llvm/bin
CC       := $(LLVM)/clang
OBJCOPY  := $(LLVM)/llvm-objcopy
LD       := ld.lld
TARGET   := --target=aarch64-none-elf

CFLAGS   := $(TARGET) -std=gnu11 -ffreestanding -fno-builtin -nostdlib -mgeneral-regs-only \
            -Wall -Wextra -Werror -O2 -g -Iinclude -MMD -MP
ASFLAGS  := $(TARGET) -g -Iinclude -MMD -MP

SRCS     := arch/aarch64/boot.S \
            kernel/main.c \
            kernel/printk.c \
            kernel/devicetree.c \
            lib/fdt.c \
            lib/string.c \
            drivers/serial/pl011.c

OBJS     := $(addprefix build/,$(addsuffix .o,$(basename $(SRCS))))
LDSCRIPT := arch/aarch64/linker.ld

QEMU     := qemu-system-aarch64 -machine virt -cpu cortex-a53 -smp 4 -m 512M -nographic

all: build/Image

build/Image: build/kernel.elf
	$(OBJCOPY) -O binary $< $@

build/kernel.elf: $(OBJS) $(LDSCRIPT)
	$(LD) -T $(LDSCRIPT) -o $@ $(OBJS)

build/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

build/%.o: %.S
	@mkdir -p $(dir $@)
	$(CC) $(ASFLAGS) -c $< -o $@

run: build/Image
	$(QEMU) -kernel $<

dumpdtb:
	@mkdir -p build
	$(QEMU) -machine dumpdtb=build/virt.dtb
	dtc -I dtb -O dts build/virt.dtb -o build/virt.dts

clean:
	rm -rf build

-include $(OBJS:.o=.d)

.PHONY: all run dumpdtb clean
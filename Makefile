# SPDX-License-Identifier: GPL-2.0-only
CC       := /opt/homebrew/opt/llvm/bin/clang
LD       := ld.lld
TARGET   := --target=aarch64-none-elf

CFLAGS   := $(TARGET) -std=gnu11 -ffreestanding -nostdlib -mgeneral-regs-only \
            -Wall -Wextra -Werror -O2 -g -Iinclude -MMD -MP
ASFLAGS  := $(TARGET) -g -Iinclude -MMD -MP

SRCS     := arch/aarch64/boot.S \
            kernel/main.c \
            kernel/printk.c \
            drivers/serial/pl011.c

OBJS     := $(addprefix build/,$(addsuffix .o,$(basename $(SRCS))))
LDSCRIPT := arch/aarch64/linker.ld

all: build/kernel.elf

build/kernel.elf: $(OBJS) $(LDSCRIPT)
	$(LD) -T $(LDSCRIPT) -o $@ $(OBJS)

build/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

build/%.o: %.S
	@mkdir -p $(dir $@)
	$(CC) $(ASFLAGS) -c $< -o $@

run: build/kernel.elf
	qemu-system-aarch64 -machine virt -cpu cortex-a53 -smp 4 -m 512M \
		-nographic -kernel $<

clean:
	rm -rf build

-include $(OBJS:.o=.d)

.PHONY: all run clean
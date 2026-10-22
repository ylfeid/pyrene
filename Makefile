# SPDX-License-Identifier: GPL-2.0-only
CC      := /opt/homebrew/opt/llvm/bin/clang
LD      := ld.lld
TARGET  := --target=aarch64-none-elf

CFLAGS  := $(TARGET) -std=gnu11 -ffreestanding -nostdlib -mgeneral-regs-only \
           -Wall -Wextra -Werror -O2 -g
ASFLAGS := $(TARGET) -g

OBJS    := build/boot.o build/main.o

all: build/kernel.elf

build:
	mkdir -p build

build/boot.o: arch/aarch64/boot.S | build
	$(CC) $(ASFLAGS) -c $< -o $@

build/main.o: kernel/main.c | build
	$(CC) $(CFLAGS) -c $< -o $@

build/kernel.elf: $(OBJS) arch/aarch64/linker.ld
	$(LD) -T arch/aarch64/linker.ld -o $@ $(OBJS)

run: build/kernel.elf
	qemu-system-aarch64 -machine virt -cpu cortex-a53 -smp 4 -m 512M \
		-nographic -kernel $<

clean:
	rm -rf build

.PHONY: all run clean
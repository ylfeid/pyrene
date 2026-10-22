/* SPDX-License-Identifier: GPL-2.0-only */
ENTRY(_start)

SECTIONS
{
    . = 0x40080000;

    .text : {
        KEEP(*(.text.boot))
        *(.text*)
    }
    .rodata : { *(.rodata*) }
    .data   : { *(.data*) }

    .bss (NOLOAD) : ALIGN(16) {
        __bss_start = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(16);
        __bss_end = .;
    }

    . = ALIGN(16);
    . += 0x4000;
    __stack_top = .;

    __kernel_end = .;
    __image_size = __kernel_end - _start;
}
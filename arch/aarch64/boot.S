// SPDX-License-Identifier: GPL-2.0-only
.section .text.boot
.global _start

_start:
    mrs     x1, mpidr_el1
    and     x1, x1, #0xff
    cbz     x1, primary

park:
    wfe
    b       park

primary:
    ldr     x1, =__stack_top
    mov     sp, x1

    ldr     x1, =__bss_start
    ldr     x2, =__bss_end
1:  cmp     x1, x2
    b.hs    2f
    str     xzr, [x1], #8
    b       1b

2:  bl      kernel_main
    b       park

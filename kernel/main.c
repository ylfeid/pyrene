// SPDX-License-Identifier: GPL-2.0-only
#include <stdint.h>
#include "pl011.h"
#include "printk.h"

extern char _start[], __bss_start[], __bss_end[], __stack_top[];

static unsigned current_el(void)
{
    uint64_t el;
    __asm__ volatile("mrs %0, CurrentEL" : "=r"(el));
    return (el >> 2) & 3;
}

void kernel_main(void)
{
    pl011_init(0x09000000);

    printk("pyrene booting\n");
    printk("  exception level: EL%u\n", current_el());
    printk("  kernel start:    %p\n", (void *)_start);
    printk("  bss:             %p - %p\n", (void *)__bss_start, (void *)__bss_end);
    printk("  stack top:       %p\n", (void *)__stack_top);
    printk("  printk test:     %d %u %x %08x %lx %s %c %%\n",
           -42, 42u, 0xbeefu, 0x1234u, 0xdeadbeefcafeUL, "ok", '!');

    for (;;)
        __asm__ volatile("wfe");
}
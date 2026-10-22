// SPDX-License-Identifier: GPL-2.0-only
#include <stdint.h>
#include "fdt.h"
#include "pl011.h"
#include "printk.h"

extern char _start[], __bss_start[], __bss_end[], __stack_top[];

static unsigned current_el(void)
{
    uint64_t el;
    __asm__ volatile("mrs %0, CurrentEL" : "=r"(el));
    return (el >> 2) & 3;
}

void kernel_main(uintptr_t dtb)
{
    pl011_init(0x09000000);

    printk("pyrene booting\n");
    printk("  exception level: EL%u\n", current_el());
    printk("  kernel start:    %p\n", (void *)_start);
    printk("  bss:             %p - %p\n", (void *)__bss_start, (void *)__bss_end);
    printk("  stack top:       %p\n", (void *)__stack_top);
    printk("  dtb:             %p\n", (void *)dtb);

    const struct fdt_header *h = (const struct fdt_header *)dtb;

    if (!dtb || fdt32_to_cpu(h->magic) != FDT_MAGIC) {
        printk("  dtb invalid or missing\n");
    } else {
        printk("  dtb size:        %u bytes\n", fdt32_to_cpu(h->totalsize));
        printk("  dtb version:     %u\n", fdt32_to_cpu(h->version));
        printk("  boot cpu:        %u\n", fdt32_to_cpu(h->boot_cpuid_phys));
    }

    for (;;)
        __asm__ volatile("wfe");
}
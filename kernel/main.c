// SPDX-License-Identifier: GPL-2.0-only
#include <stdint.h>
#include "devicetree.h"
#include "panic.h"
#include "pl011.h"
#include "printk.h"
#include "sysreg.h"

#define EARLY_UART  0x09000000UL

extern char _start[], __kernel_end[];

static unsigned current_el(void)
{
    return (read_sysreg(CurrentEL) >> 2) & 3;
}

static void halt(void)
{
    for (;;)
        __asm__ volatile("wfe");
}

void kernel_main(uintptr_t dtb)
{
    struct dt_info info;

    pl011_init(EARLY_UART);

    printk("pyrene booting\n");
    printk("  exception level: EL%u\n", current_el());
    printk("  kernel image:    %p - %p\n", (void *)_start, (void *)__kernel_end);
    printk("  dtb:             %p\n", (void *)dtb);

    if (dt_parse((const void *)dtb, &info))
        panic("invalid devicetree at %p", (void *)dtb);

    if (info.has_uart)
        pl011_init(info.uart_base);

    printk("  console:         pl011 @ %p\n", (void *)(uintptr_t)info.uart_base);

    if (info.stdout_path)
        printk("  stdout-path:     %s\n", info.stdout_path);
    if (info.bootargs)
        printk("  bootargs:        %s\n", info.bootargs);
        
    printk("  cpus:            %u\n", info.ncpus);
    for (unsigned i = 0; i < info.nram; i++)
        printk("  ram:             %p - %p (%lu MiB)\n",
               (void *)(uintptr_t)info.ram[i].base,
               (void *)(uintptr_t)(info.ram[i].base + info.ram[i].size),
               info.ram[i].size >> 20);
    if (!info.nreserved)
        printk("  reserved:        none\n");
    for (unsigned i = 0; i < info.nreserved; i++)
        printk("  reserved:        %p - %p\n",
               (void *)(uintptr_t)info.reserved[i].base,
               (void *)(uintptr_t)(info.reserved[i].base + info.reserved[i].size));
    halt();
}
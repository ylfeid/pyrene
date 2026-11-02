// SPDX-License-Identifier: GPL-2.0-only
#include <stdarg.h>
#include "panic.h"
#include "printk.h"

void panic(const char *fmt, ...)
{
    va_list ap;

    __asm__ volatile("msr daifset, #0xf");

    printk("\nKERNEL PANIC: ");
    va_start(ap, fmt);
    vprintk(fmt, ap);
    va_end(ap);
    printk("\n");

    for (;;)
        __asm__ volatile("wfe");
}
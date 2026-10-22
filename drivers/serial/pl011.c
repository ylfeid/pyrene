// SPDX-License-Identifier: GPL-2.0-only
#include <stdint.h>
#include "io.h"
#include "pl011.h"

#define UART_DR     0x00
#define UART_FR     0x18
#define FR_TXFF     (1u << 5)

static uintptr_t uart_base;

void pl011_init(uintptr_t base)
{
    uart_base = base;
}

void pl011_putc(char c)
{
    while (mmio_read32(uart_base + UART_FR) & FR_TXFF)
        ;
    mmio_write32(uart_base + UART_DR, (uint32_t)c);
}
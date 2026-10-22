// SPDX-License-Identifier: GPL-2.0-only
#include <stdint.h>

#define UART0       0x09000000UL
#define UART_DR     0x00
#define UART_FR     0x18
#define FR_TXFF     (1u << 5)

static inline void mmio_write32(uintptr_t addr, uint32_t val)
{
    *(volatile uint32_t *)addr = val;
}

static inline uint32_t mmio_read32(uintptr_t addr)
{
    return *(volatile uint32_t *)addr;
}

static void uart_putc(char c)
{
    while (mmio_read32(UART0 + UART_FR) & FR_TXFF)
        ;
    mmio_write32(UART0 + UART_DR, (uint32_t)c);
}

static void uart_puts(const char *s)
{
    while (*s) {
        if (*s == '\n')
            uart_putc('\r');
        uart_putc(*s++);
    }
}

void kernel_main(void)
{
    uart_puts("pyrene: hello from bare metal\n");
    for (;;)
        __asm__ volatile("wfe");
}
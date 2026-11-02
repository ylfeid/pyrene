// SPDX-License-Identifier: GPL-2.0-only
#include <stdint.h>
#include "io.h"
#include "pl011.h"

#define UART_DR     0x00
#define UART_FR     0x18
#define UART_LCRH   0x2c
#define UART_CR     0x30

#define FR_BUSY     (1u << 3)
#define FR_TXFF     (1u << 5)

#define LCRH_FEN    (1u << 4)
#define LCRH_WLEN8  (3u << 5)

#define CR_UARTEN   (1u << 0)
#define CR_TXE      (1u << 8)
#define CR_RXE      (1u << 9)

static uintptr_t uart_base;

void pl011_init(uintptr_t base)
{
    uart_base = base;

    while (mmio_read32(base + UART_FR) & FR_BUSY)
        ;

    mmio_write32(base + UART_CR, 0);
    mmio_write32(base + UART_LCRH, 0);
    mmio_write32(base + UART_LCRH, LCRH_WLEN8 | LCRH_FEN);
    mmio_write32(base + UART_CR, CR_UARTEN | CR_TXE | CR_RXE);
}

void pl011_putc(char c)
{
    while (mmio_read32(uart_base + UART_FR) & FR_TXFF)
        ;
    mmio_write32(uart_base + UART_DR, (uint32_t)c);
}
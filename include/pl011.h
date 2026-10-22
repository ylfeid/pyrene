/* SPDX-License-Identifier: GPL-2.0-only */
#pragma once
#include <stdint.h>

void pl011_init(uintptr_t base);
void pl011_putc(char c);
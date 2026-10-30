/* SPDX-License-Identifier: GPL-2.0-only */
#pragma once
#include <stdarg.h>

void printk(const char *fmt, ...) __attribute__((format(printf, 1, 2)));
void vprintk(const char *fmt, va_list ap) __attribute__((format(printf, 1, 0)));
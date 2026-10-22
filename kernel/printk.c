// SPDX-License-Identifier: GPL-2.0-only
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include "pl011.h"
#include "printk.h"

static void out_char(char c)
{
    if (c == '\n')
        pl011_putc('\r');
    pl011_putc(c);
}

static void out_str(const char *s)
{
    if (!s)
        s = "(null)";
    while (*s)
        out_char(*s++);
}

static void out_num(uint64_t val, unsigned base, bool upper, bool neg,
                    int width, char pad)
{
    const char *digits = upper ? "0123456789ABCDEF" : "0123456789abcdef";
    char buf[24];
    int i = 0;

    do {
        buf[i++] = digits[val % base];
        val /= base;
    } while (val);

    int len = i + (neg ? 1 : 0);

    if (neg && pad == '0')
        out_char('-');
    for (; len < width; len++)
        out_char(pad);
    if (neg && pad != '0')
        out_char('-');

    while (i--)
        out_char(buf[i]);
}

void printk(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);

    for (; *fmt; fmt++) {
        if (*fmt != '%') {
            out_char(*fmt);
            continue;
        }
        fmt++;

        char pad = ' ';
        int width = 0;
        int lng = 0;

        if (*fmt == '0') {
            pad = '0';
            fmt++;
        }
        while (*fmt >= '0' && *fmt <= '9') {
            width = width * 10 + (*fmt - '0');
            fmt++;
        }
        while (*fmt == 'l') {
            lng++;
            fmt++;
        }
        if (*fmt == 'z') {
            lng = 1;
            fmt++;
        }

        switch (*fmt) {
        case 'c':
            out_char((char)va_arg(ap, int));
            break;
        case 's':
            out_str(va_arg(ap, const char *));
            break;
        case 'd':
        case 'i': {
            int64_t v = lng ? va_arg(ap, long) : va_arg(ap, int);
            bool neg = v < 0;
            uint64_t u = neg ? -(uint64_t)v : (uint64_t)v;
            out_num(u, 10, false, neg, width, pad);
            break;
        }
        case 'u':
        case 'x':
        case 'X': {
            uint64_t v = lng ? va_arg(ap, unsigned long) : va_arg(ap, unsigned int);
            out_num(v, *fmt == 'u' ? 10 : 16, *fmt == 'X', false, width, pad);
            break;
        }
        case 'p':
            out_str("0x");
            out_num((uintptr_t)va_arg(ap, void *), 16, false, false, 16, '0');
            break;
        case '%':
            out_char('%');
            break;
        case '\0':
            goto done;
        default:
            out_char('%');
            out_char(*fmt);
            break;
        }
    }
done:
    va_end(ap);
}
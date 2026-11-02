/* SPDX-License-Identifier: GPL-2.0-only */
#pragma once
#include <stdint.h>

#define read_sysreg(reg) ({                                         \
    uint64_t __v;                                                   \
    __asm__ volatile("mrs %0, " #reg : "=r"(__v));                  \
    __v;                                                            \
})

#define write_sysreg(reg, val) do {                                 \
    uint64_t __v = (val);                                           \
    __asm__ volatile("msr " #reg ", %0" :: "r"(__v) : "memory");    \
} while (0)
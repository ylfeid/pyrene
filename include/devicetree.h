/* SPDX-License-Identifier: GPL-2.0-only */
#pragma once
#include <stdint.h>

#define DT_MAX_REGIONS 8

struct dt_region {
    uint64_t base;
    uint64_t size;
};

struct dt_info {
    struct dt_region ram[DT_MAX_REGIONS];
    unsigned nram;
    unsigned ncpus;
    int has_uart;
    uint64_t uart_base;
};

int dt_parse(const void *fdt, struct dt_info *info);
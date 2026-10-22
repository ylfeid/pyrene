/* SPDX-License-Identifier: GPL-2.0-only */
#pragma once
#include <stdint.h>

#define FDT_MAGIC 0xd00dfeed

struct fdt_header {
    uint32_t magic;
    uint32_t totalsize;
    uint32_t off_dt_struct;
    uint32_t off_dt_strings;
    uint32_t off_mem_rsvmap;
    uint32_t version;
    uint32_t last_comp_version;
    uint32_t boot_cpuid_phys;
    uint32_t size_dt_strings;
    uint32_t size_dt_struct;
};

static inline uint32_t fdt32_to_cpu(uint32_t x)
{
    return __builtin_bswap32(x);
}
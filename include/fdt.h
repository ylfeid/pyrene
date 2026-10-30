/* SPDX-License-Identifier: GPL-2.0-only */
#pragma once
#include <stdint.h>

#define FDT_MAGIC       0xd00dfeed

#define FDT_BEGIN_NODE  1
#define FDT_END_NODE    2
#define FDT_PROP        3
#define FDT_NOP         4
#define FDT_END         9

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

struct fdt_node {
    const char *name;
    int depth;
    unsigned addr_cells;
    unsigned size_cells;
    unsigned parent_addr_cells;
    unsigned parent_size_cells;
};

typedef int (*fdt_node_fn)(void *arg, const struct fdt_node *node);
typedef int (*fdt_prop_fn)(void *arg, const struct fdt_node *node,
                           const char *name, const void *data, uint32_t len);

static inline uint32_t fdt32_to_cpu(uint32_t x)
{
    return __builtin_bswap32(x);
}

int fdt_check(const void *fdt);
int fdt_walk(const void *fdt, fdt_node_fn on_node, fdt_prop_fn on_prop, void *arg);
uint64_t fdt_read_cells(const void *data, unsigned cells);
int fdt_compatible(const void *data, uint32_t len, const char *compat);
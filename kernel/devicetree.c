// SPDX-License-Identifier: GPL-2.0-only
#include <stdint.h>
#include "devicetree.h"
#include "fdt.h"
#include "string.h"

struct dt_state {
    struct dt_info *info;
    int is_memory;
    int is_uart;
    int is_chosen;
    int have_reg;
    uint64_t reg_base;
};

static int name_is(const char *name, const char *base)
{
    size_t n = strlen(base);

    return !strncmp(name, base, n) && (name[n] == '\0' || name[n] == '@');
}

static int on_node(void *arg, const struct fdt_node *node)
{
    struct dt_state *st = arg;

    st->is_memory = node->depth == 1 && name_is(node->name, "memory");
    st->is_uart = 0;
    st->is_chosen = node->depth == 1 && name_is(node->name, "chosen");
    st->have_reg = 0;

    if (node->depth == 2 && !strncmp(node->name, "cpu@", 4))
        st->info->ncpus++;
    return 0;
}

static int on_prop(void *arg, const struct fdt_node *node, const char *name,
                   const void *data, uint32_t len)
{
    struct dt_state *st = arg;
    struct dt_info *info = st->info;
    
    if (st->is_chosen) {
        if (!strcmp(name, "bootargs"))
            info->bootargs = data;
        else if (!strcmp(name, "stdout-path"))
            info->stdout_path = data;
    }

    if (!strcmp(name, "reg")) {
        unsigned a = node->parent_addr_cells;
        unsigned s = node->parent_size_cells;
        uint32_t stride = (a + s) * 4;

        if (st->is_memory && stride) {
            for (uint32_t off = 0; off + stride <= len && info->nram < DT_MAX_REGIONS; off += stride) {
                const uint8_t *e = (const uint8_t *)data + off;

                info->ram[info->nram].base = fdt_read_cells(e, a);
                info->ram[info->nram].size = fdt_read_cells(e + a * 4, s);
                info->nram++;
            }
        } else if (len >= a * 4) {
            st->have_reg = 1;
            st->reg_base = fdt_read_cells(data, a);
        }
    } else if (!strcmp(name, "compatible")) {
        if (fdt_compatible(data, len, "arm,pl011"))
            st->is_uart = 1;
    }

    if (st->is_uart && st->have_reg && !info->has_uart) {
        info->uart_base = st->reg_base;
        info->has_uart = 1;
    }
    return 0;
}

int dt_parse(const void *fdt, struct dt_info *info)
{
    struct dt_state st = { .info = info };
    uint64_t addr, size;

    memset(info, 0, sizeof(*info));
    if (fdt_check(fdt))
        return -1;

    info->fdt.base = (uintptr_t)fdt;
    info->fdt.size = fdt32_to_cpu(((const struct fdt_header *)fdt)->totalsize);

    for (unsigned i = 0; info->nreserved < DT_MAX_REGIONS &&
                         !fdt_memreserve(fdt, i, &addr, &size); i++) {
        info->reserved[info->nreserved].base = addr;
        info->reserved[info->nreserved].size = size;
        info->nreserved++;
    }

    return fdt_walk(fdt, on_node, on_prop, &st);
}
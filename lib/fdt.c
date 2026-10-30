// SPDX-License-Identifier: GPL-2.0-only
#include <stddef.h>
#include <stdint.h>
#include "fdt.h"
#include "string.h"

#define FDT_MAX_DEPTH 16

int fdt_check(const void *fdt)
{
    const struct fdt_header *h = fdt;

    if (!fdt || ((uintptr_t)fdt & 7))
        return -1;
    if (fdt32_to_cpu(h->magic) != FDT_MAGIC)
        return -1;
    if (fdt32_to_cpu(h->version) < 17)
        return -1;
    if (fdt32_to_cpu(h->last_comp_version) > 17)
        return -1;
    return 0;
}

uint64_t fdt_read_cells(const void *data, unsigned cells)
{
    const uint32_t *p = data;
    uint64_t v = 0;

    while (cells--)
        v = (v << 32) | fdt32_to_cpu(*p++);
    return v;
}

int fdt_compatible(const void *data, uint32_t len, const char *compat)
{
    const char *s = data;
    const char *end = s + len;
    size_t n = strlen(compat);

    while (s < end) {
        size_t l = strlen(s);

        if (l == n && !memcmp(s, compat, n))
            return 1;
        s += l + 1;
    }
    return 0;
}

int fdt_walk(const void *fdt, fdt_node_fn on_node, fdt_prop_fn on_prop, void *arg)
{
    const struct fdt_header *h = fdt;
    const uint32_t *p = (const uint32_t *)((const char *)fdt + fdt32_to_cpu(h->off_dt_struct));
    const uint32_t *end = p + fdt32_to_cpu(h->size_dt_struct) / 4;
    const char *strings = (const char *)fdt + fdt32_to_cpu(h->off_dt_strings);
    struct fdt_node stack[FDT_MAX_DEPTH];
    int depth = -1;

    while (p < end) {
        uint32_t tok = fdt32_to_cpu(*p++);

        switch (tok) {
        case FDT_BEGIN_NODE: {
            const char *name = (const char *)p;
            size_t len = strlen(name) + 1;
            struct fdt_node *n;

            p += (len + 3) / 4;
            depth++;
            if (depth >= FDT_MAX_DEPTH)
                return -1;

            n = &stack[depth];
            n->name = name;
            n->depth = depth;
            n->addr_cells = 2;
            n->size_cells = 1;
            n->parent_addr_cells = depth ? stack[depth - 1].addr_cells : 2;
            n->parent_size_cells = depth ? stack[depth - 1].size_cells : 1;

            if (on_node && on_node(arg, n))
                return 0;
            break;
        }
        case FDT_END_NODE:
            if (depth < 0)
                return -1;
            depth--;
            break;
        case FDT_PROP: {
            uint32_t len, nameoff;
            const char *name;
            const void *data;

            if (depth < 0 || p + 2 > end)
                return -1;
            len = fdt32_to_cpu(p[0]);
            nameoff = fdt32_to_cpu(p[1]);
            name = strings + nameoff;
            data = &p[2];
            p += 2 + (len + 3) / 4;

            if (!strcmp(name, "#address-cells") && len == 4)
                stack[depth].addr_cells = fdt32_to_cpu(*(const uint32_t *)data);
            else if (!strcmp(name, "#size-cells") && len == 4)
                stack[depth].size_cells = fdt32_to_cpu(*(const uint32_t *)data);

            if (on_prop && on_prop(arg, &stack[depth], name, data, len))
                return 0;
            break;
        }
        case FDT_NOP:
            break;
        case FDT_END:
            return 0;
        default:
            return -1;
        }
    }
    return -1;
}
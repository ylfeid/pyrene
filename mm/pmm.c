// SPDX-License-Identifier: GPL-2.0-only
#include <stddef.h>
#include <stdint.h>
#include "bitmap.h"
#include "kernel.h"
#include "panic.h"
#include "pmm.h"
#include "string.h"

extern char __kernel_end[];

static uint64_t *bitmap;
static phys_addr_t base;
static size_t npages;
static size_t nfree;

static void range_free(phys_addr_t start, phys_addr_t end)
{
    phys_addr_t limit = base + ((phys_addr_t)npages << PAGE_SHIFT);

    start = MAX(ALIGN_UP(start, PAGE_SIZE), base);
    end = MIN(ALIGN_DOWN(end, PAGE_SIZE), limit);

    for (phys_addr_t pa = start; pa < end; pa += PAGE_SIZE) {
        size_t pfn = (pa - base) >> PAGE_SHIFT;

        if (bitmap_test(bitmap, pfn)) {
            bitmap_clear(bitmap, pfn);
            nfree++;
        }
    }
}

void pmm_init(const struct dt_info *info)
{
    phys_addr_t lo = UINT64_MAX, hi = 0;

    if (!info->nram)
        panic("pmm: no memory in devicetree");

    for (unsigned i = 0; i < info->nram; i++) {
        lo = MIN(lo, info->ram[i].base);
        hi = MAX(hi, info->ram[i].base + info->ram[i].size);
    }

    base = ALIGN_DOWN(lo, PAGE_SIZE);
    npages = (ALIGN_UP(hi, PAGE_SIZE) - base) >> PAGE_SHIFT;
    bitmap = (uint64_t *)ALIGN_UP((uintptr_t)__kernel_end, PAGE_SIZE);
    memset(bitmap, 0xff, BITMAP_WORDS(npages) * sizeof(uint64_t));

    for (unsigned i = 0; i < info->nram; i++)
        range_free(info->ram[i].base, info->ram[i].base + info->ram[i].size);
}

size_t pmm_total_pages(void)
{
    return npages;
}

size_t pmm_free_pages(void)
{
    return nfree;
}

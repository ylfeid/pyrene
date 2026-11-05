/* SPDX-License-Identifier: GPL-2.0-only */
#pragma once
#include <stddef.h>
#include <stdint.h>
#include "devicetree.h"

#define PAGE_SHIFT  12
#define PAGE_SIZE   (1UL << PAGE_SHIFT)

typedef uint64_t phys_addr_t;

void pmm_init(const struct dt_info *info);
size_t pmm_total_pages(void);
size_t pmm_free_pages(void);

/* SPDX-License-Identifier: GPL-2.0-only */
#pragma once
#include <stddef.h>
#include <stdint.h>

#define BITS_PER_WORD       64
#define BITMAP_WORDS(n)     (((n) + BITS_PER_WORD - 1) / BITS_PER_WORD)

static inline void bitmap_set(uint64_t *map, size_t bit)
{
    map[bit / BITS_PER_WORD] |= 1UL << (bit % BITS_PER_WORD);
}

static inline void bitmap_clear(uint64_t *map, size_t bit)
{
    map[bit / BITS_PER_WORD] &= ~(1UL << (bit % BITS_PER_WORD));
}

static inline int bitmap_test(const uint64_t *map, size_t bit)
{
    return (map[bit / BITS_PER_WORD] >> (bit % BITS_PER_WORD)) & 1;
}

size_t bitmap_find_clear_run(const uint64_t *map, size_t nbits, size_t start, size_t count);

// SPDX-License-Identifier: GPL-2.0-only
#include <stddef.h>
#include <stdint.h>
#include "bitmap.h"

size_t bitmap_find_clear_run(const uint64_t *map, size_t nbits, size_t start, size_t count)
{
    size_t run = 0;

    if (!count)
        return nbits;

    for (size_t bit = start; bit < nbits; bit++) {
        if (!(bit % BITS_PER_WORD) && map[bit / BITS_PER_WORD] == ~0UL) {
            run = 0;
            bit += BITS_PER_WORD - 1;
            continue;
        }
        if (bitmap_test(map, bit)) {
            run = 0;
            continue;
        }
        if (++run == count)
            return bit + 1 - count;
    }
    return nbits;
}

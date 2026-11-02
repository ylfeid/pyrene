/* SPDX-License-Identifier: GPL-2.0-only */
#pragma once
#include <stdint.h>

struct trap_frame {
    uint64_t x[31];
    uint64_t sp;
    uint64_t elr;
    uint64_t spsr;
};

_Static_assert(sizeof(struct trap_frame) == 272, "trap frame size must match vectors.S");

void exceptions_init(void);
void trap_handler(struct trap_frame *tf, unsigned type);
/* SPDX-License-Identifier: GPL-2.0-only */
#pragma once

#define ARRAY_SIZE(a)       (sizeof(a) / sizeof((a)[0]))

#define ALIGN_UP(x, a)      (((x) + ((a) - 1)) & ~((__typeof__(x))(a) - 1))
#define ALIGN_DOWN(x, a)    ((x) & ~((__typeof__(x))(a) - 1))
#define IS_ALIGNED(x, a)    (((x) & ((__typeof__(x))(a) - 1)) == 0)

#define MIN(a, b)           ((a) < (b) ? (a) : (b))
#define MAX(a, b)           ((a) > (b) ? (a) : (b))
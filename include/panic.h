/* SPDX-License-Identifier: GPL-2.0-only */
#pragma once

void panic(const char *fmt, ...) __attribute__((noreturn, format(printf, 1, 2)));
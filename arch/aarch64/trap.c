// SPDX-License-Identifier: GPL-2.0-only
#include <stdint.h>
#include "esr.h"
#include "panic.h"
#include "printk.h"
#include "sysreg.h"
#include "trap.h"

extern char vectors[];

static const char *const vector_names[16] = {
    "sync (el1t)",  "irq (el1t)",  "fiq (el1t)",  "serror (el1t)",
    "sync (el1h)",  "irq (el1h)",  "fiq (el1h)",  "serror (el1h)",
    "sync (el0 64)", "irq (el0 64)", "fiq (el0 64)", "serror (el0 64)",
    "sync (el0 32)", "irq (el0 32)", "fiq (el0 32)", "serror (el0 32)",
};

static const char *const ec_names[64] = {
    [ESR_EC_UNKNOWN]     = "unknown reason",
    [ESR_EC_WFX]         = "wfi/wfe",
    [ESR_EC_FP_ASIMD]    = "fp/simd access",
    [ESR_EC_ILLEGAL]     = "illegal execution state",
    [ESR_EC_SVC64]       = "svc",
    [ESR_EC_SYS64]       = "msr/mrs/sys",
    [ESR_EC_IABT_LOW]    = "instruction abort (lower el)",
    [ESR_EC_IABT_CUR]    = "instruction abort",
    [ESR_EC_PC_ALIGN]    = "pc alignment fault",
    [ESR_EC_DABT_LOW]    = "data abort (lower el)",
    [ESR_EC_DABT_CUR]    = "data abort",
    [ESR_EC_SP_ALIGN]    = "sp alignment fault",
    [ESR_EC_SERROR]      = "serror",
    [ESR_EC_BREAKPT_LOW] = "breakpoint (lower el)",
    [ESR_EC_BREAKPT_CUR] = "breakpoint",
    [ESR_EC_SOFTSTP_LOW] = "software step (lower el)",
    [ESR_EC_SOFTSTP_CUR] = "software step",
    [ESR_EC_WATCHPT_LOW] = "watchpoint (lower el)",
    [ESR_EC_WATCHPT_CUR] = "watchpoint",
    [ESR_EC_BRK64]       = "brk",
};

static int far_valid(unsigned ec)
{
    switch (ec) {
    case ESR_EC_IABT_LOW:
    case ESR_EC_IABT_CUR:
    case ESR_EC_PC_ALIGN:
    case ESR_EC_DABT_LOW:
    case ESR_EC_DABT_CUR:
    case ESR_EC_WATCHPT_LOW:
    case ESR_EC_WATCHPT_CUR:
        return 1;
    default:
        return 0;
    }
}

void exceptions_init(void)
{
    write_sysreg(vbar_el1, (uint64_t)vectors);
    __asm__ volatile("isb" ::: "memory");
}

void trap_handler(struct trap_frame *tf, unsigned type)
{
    uint64_t esr = read_sysreg(esr_el1);
    uint64_t far = read_sysreg(far_el1);
    unsigned ec = ESR_EC(esr);
    unsigned kind = type & 3;

    printk("\nexception: %s\n", vector_names[type & 15]);
    if (kind == 0 || kind == 3) {
        printk("  esr: 0x%08lx  ec 0x%02x %s, iss 0x%07lx\n",
               esr, ec, ec_names[ec] ? ec_names[ec] : "reserved", ESR_ISS(esr));
        if (far_valid(ec))
            printk("  far: %p\n", (void *)far);
    }
    printk("  elr: %p  spsr: 0x%08lx\n", (void *)tf->elr, tf->spsr);
    panic("unhandled exception");
}

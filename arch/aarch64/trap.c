#include <limits.h>
#include <stdint.h>
#include "panic.h"
#include "sysreg.h"
#include "trap.h"

extern char vectors[];

void exceptions_init(void)
{
    write_sysreg(vbar_el1, (uint64_t)vectors);
    __asm__ volatile("isb" ::: "memory");
}

void trap_handler(struct trap_frame *tf, unsigned type)
{
    panic("unhandled exception, vector %u at %p", type, (void *)tf->elr);
}

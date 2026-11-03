/* SPDX-License-Identifier: GPL-2.0-only */
#pragma once

#define ESR_EC_SHIFT        26
#define ESR_EC_MASK         0x3fUL
#define ESR_IL              (1UL << 25)
#define ESR_ISS_MASK        0x1ffffffUL

#define ESR_EC(esr)         (((esr) >> ESR_EC_SHIFT) & ESR_EC_MASK)
#define ESR_ISS(esr)        ((esr) & ESR_ISS_MASK)

#define ESR_EC_UNKNOWN      0x00
#define ESR_EC_WFX          0x01
#define ESR_EC_FP_ASIMD     0x07
#define ESR_EC_ILLEGAL      0x0e
#define ESR_EC_SVC64        0x15
#define ESR_EC_SYS64        0x18
#define ESR_EC_IABT_LOW     0x20
#define ESR_EC_IABT_CUR     0x21
#define ESR_EC_PC_ALIGN     0x22
#define ESR_EC_DABT_LOW     0x24
#define ESR_EC_DABT_CUR     0x25
#define ESR_EC_SP_ALIGN     0x26
#define ESR_EC_SERROR       0x2f
#define ESR_EC_BREAKPT_LOW  0x30
#define ESR_EC_BREAKPT_CUR  0x31
#define ESR_EC_SOFTSTP_LOW  0x32
#define ESR_EC_SOFTSTP_CUR  0x33
#define ESR_EC_WATCHPT_LOW  0x34
#define ESR_EC_WATCHPT_CUR  0x35
#define ESR_EC_BRK64        0x3c

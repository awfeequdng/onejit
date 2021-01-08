// +build gc,arm64

/*
 * gomacro - A Go interpreter with Lisp-like macros
 *
 * Copyright (C) 2019 Massimiliano Ghilardi
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * misc_arm64_test.s
 *
 *  Created on Oct 27, 2019
 *      Author Massimiliano Ghilardi
 */

#include "funcdata.h"  // for NO_LOCAL_POINTERS
#include "textflag.h"  // for NOSPLIT

TEXT ·asm_loop(SB), NOSPLIT | NOFRAME,
    $0 -
        0 label_asm_loop : JMP label_asm_loop RET

                           DATA ·const_canary
                           +
                           0(SB) / 8,
    $·canary(SB) GLOBL ·const_canary + 0(SB), RODATA,
    $8

    TEXT ·asm_address_of_canary(SB),
    NOSPLIT | NOFRAME, $0 - 8 MOVD $·const_canary(SB),
    R0  // closure. actual function is MOVD $·canary(SB), R0
    MOVD R0,
    ret + 0(FP)RET

        TEXT ·asm_call_canary(SB),
    NOSPLIT, $8 - 8 NO_LOCAL_POINTERS MOVD received_arg + 0(FP), R0 MOVD R0,
    local_arg - 8(SP)CALL ·canary(SB) RET

        TEXT ·asm_call_func(SB),
    NOSPLIT, $8 - 16 NO_LOCAL_POINTERS MOVD func_address + 0(FP),
    R0 MOVD received_arg + 8(FP), R1 MOVD R1,
    local_arg - 8(SP)CALL R0  // same as CALL (R0)
        RET

        // on amd64, closure itself must be passed in R26
        TEXT ·asm_call_closure(SB),
    NOSPLIT, $8 - 16 NO_LOCAL_POINTERS MOVD closure + 0(FP),
    R26 MOVD received_arg + 8(FP), R0 MOVD(R26),
    R1  // func address
    MOVD R0,
    local_arg - 8(SP)CALL R1 RET

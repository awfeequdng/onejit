// +build gc,amd64

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
 * misc_amd64_test.s
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
    NOSPLIT | NOFRAME, $0 - 8 LEAQ ·const_canary(SB),
    AX  // closure. actual function is LEAQ ·canary(SB), AX
    MOVQ AX,
    ret + 0(FP)RET

        TEXT ·asm_call_canary(SB),
    NOSPLIT, $8 - 8 NO_LOCAL_POINTERS MOVQ received_arg + 0(FP), AX MOVQ AX,
    local_arg - 8(SP)CALL ·canary(SB) RET

        TEXT ·asm_call_func(SB),
    NOSPLIT, $8 - 16 NO_LOCAL_POINTERS MOVQ func_address + 0(FP),
    AX MOVQ received_arg + 8(FP), DX MOVQ DX,
    local_arg - 8(SP)CALL AX RET

        // on amd64, closure itself must be passed in DX
        TEXT ·asm_call_closure(SB),
    NOSPLIT, $8 - 16 NO_LOCAL_POINTERS MOVQ closure + 0(FP),
    DX MOVQ received_arg + 8(FP), AX MOVQ AX, local_arg - 8(SP)CALL 0(DX)RET

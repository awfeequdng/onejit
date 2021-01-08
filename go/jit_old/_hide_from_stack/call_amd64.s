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
 * call_amd64.s
 *
 *  Created on Oct 27, 2019
 *      Author Massimiliano Ghilardi
 */

#include "funcdata.h"  // for NO_LOCAL_POINTERS
#include "textflag.h"  // for NOSPLIT

TEXT ·GrowStack(SB), 0,
    $640 - 0 NO_LOCAL_POINTERS RET

        // hidden JIT functions will be replaced by this function in the
        // stacktrace
        TEXT ·hidden_jit_func(SB),
    NOSPLIT | NOFRAME,
    $0 - 8  // same signature as asm_hideme
        NO_LOCAL_POINTERS RET

        /**
         * call the closure stored in DX (which expects exactly 0 bytes of
         * arguments
         * + return values), hiding the caller from runtime stack:
         * caller is replaced with a fake entry hidden_jit_func()
         */
        TEXT ·call0(SB),
    NOSPLIT, $8 - 0 NO_LOCAL_POINTERS LEAQ ·hidden_jit_func(SB),
    BX MOVQ caller_ip + 8(SP), AX MOVQ BX, caller_ip + 8(SP)MOVQ AX,
    save_caller_ip -
        8(SP)
        /*
         * Go call abi: closures are pointers to
         * struct {
         *   func_address uintptr
         *   closure_data [...]uint8
         * }
         * and struct address must be passed in DX
         */
        CALL 0(DX)MOVQ save_caller_ip
        - 8(SP),
    AX MOVQ AX,
    caller_ip + 8(SP)RET

        /**
         * call the closure stored in DX (which expects up to 16 bytes of
         * arguments
         * + return values), hiding the caller from runtime stack:
         * caller is replaced with a fake entry hidden_jit_func()
         *
         * writing arguments in our stack and retrieving return values from it
         * are caller's responsibility
         * (possible only in assembly: caller has to access our stack)
         */
        TEXT ·call16(SB),
    NOSPLIT, $24 - 0 NO_LOCAL_POINTERS LEAQ ·hidden_jit_func(SB),
    BX MOVQ caller_ip + 8(SP), AX MOVQ BX, caller_ip + 8(SP)MOVQ AX,
    save_caller_ip - 8(SP)CALL 0(DX)MOVQ save_caller_ip - 8(SP), AX MOVQ AX,
    caller_ip + 8(SP)RET

        // as above, but closure can expect up to 32 bytes of arguments return
        // values
        TEXT ·call32(SB),
    NOSPLIT, $40 - 0 NO_LOCAL_POINTERS LEAQ ·hidden_jit_func(SB),
    BX MOVQ caller_ip + 8(SP), AX MOVQ BX, caller_ip + 8(SP)MOVQ AX,
    save_caller_ip - 8(SP)CALL 0(DX)MOVQ save_caller_ip - 8(SP), AX MOVQ AX,
    caller_ip + 8(SP)RET

        // as above, but closure can expect up to 64 bytes of arguments return
        // values
        TEXT ·call64(SB),
    NOSPLIT, $72 - 0 NO_LOCAL_POINTERS LEAQ ·hidden_jit_func(SB),
    BX MOVQ caller_ip + 8(SP), AX MOVQ BX, caller_ip + 8(SP)MOVQ AX,
    save_caller_ip - 8(SP)CALL 0(DX)MOVQ save_caller_ip - 8(SP), AX MOVQ AX,
    caller_ip + 8(SP)RET

        // as above, but closure can expect up to 128 bytes of arguments return
        // values
        TEXT ·call128(SB),
    NOSPLIT, $136 - 0 NO_LOCAL_POINTERS LEAQ ·hidden_jit_func(SB),
    BX MOVQ caller_ip + 8(SP), AX MOVQ BX, caller_ip + 8(SP)MOVQ AX,
    save_caller_ip - 8(SP)CALL 0(DX)MOVQ save_caller_ip - 8(SP), AX MOVQ AX,
    caller_ip + 8(SP)RET

        // as above, but closure can expect up to 256 bytes of arguments return
        // values
        TEXT ·call256(SB),
    NOSPLIT, $264 - 0 NO_LOCAL_POINTERS LEAQ ·hidden_jit_func(SB),
    BX MOVQ caller_ip + 8(SP), AX MOVQ BX, caller_ip + 8(SP)MOVQ AX,
    save_caller_ip - 8(SP)CALL 0(DX)MOVQ save_caller_ip - 8(SP), AX MOVQ AX,
    caller_ip + 8(SP)RET

        // as above, but closure can expect up to 512 bytes of arguments return
        // values
        TEXT ·call512(SB),
    NOSPLIT, $520 - 0 NO_LOCAL_POINTERS LEAQ ·hidden_jit_func(SB),
    BX MOVQ caller_ip + 8(SP), AX MOVQ BX, caller_ip + 8(SP)MOVQ AX,
    save_caller_ip - 8(SP)CALL 0(DX)MOVQ save_caller_ip - 8(SP), AX MOVQ AX,
    caller_ip + 8(SP)RET

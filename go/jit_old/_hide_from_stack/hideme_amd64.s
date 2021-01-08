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
 * hideme_amd64.s
 *
 *  Created on Oct 27, 2019
 *      Author Massimiliano Ghilardi
 */

#include "funcdata.h"  // for NO_LOCAL_POINTERS
#include "textflag.h"  // for NOSPLIT

// use same restrictions as a JIT function:
// cannot have stack map, frame, local variables;
// must use one of call* trampolines to invoke arbitrary Go functions
TEXT ·asm_hideme(SB), NOSPLIT | NOFRAME,
    $0 -
        8  // must not have local variables
        MOVQ env
        + 0(FP),
    AX MOVQ 0(AX),
    DX  // closure, must be in DX
    MOVQ 8(AX),
    BX  // closure arg
    MOVQ 24(AX),
    CX  // helper function: use call[1] == call16
    MOVQ BX,
    local_arg - 40(SP)  // write into callee stack
        CALL CX RET

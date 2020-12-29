/*
 * onejit - JIT compiler in Go
 *
 * Copyright (C) 2018-2020 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * asm_op0.go
 *
 *  Created on Jan 27, 2019
 *      Author Massimiliano Ghilardi
 */

package amd64

import (
	. "github.com/cosmos72/onejit/jit"
)

// ============================================================================
// no-arg instruction

func op0(asm *Assembler, op Op) *Assembler {
	switch op {
	case BADOP:
		asm.Bytes(0x0F, 0x0B) // UD2
	case RET:
		asm.Byte(0xC3)
	case NOOP:
		asm.Byte(0x90)
	default:
		Errorf("unknown op0 instruction: %v", op)
	}
	return asm
}

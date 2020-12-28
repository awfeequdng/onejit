/*
 * gomacrojit - JIT compiler in Go
 *
 * Copyright (C) 2018-2020 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * asm_op1.go
 *
 *  Created on Jan 23, 2019
 *      Author Massimiliano Ghilardi
 */

package amd64

import (
	. "github.com/cosmos72/gomacrojit/jit"
)

func zeroReg(asm *Assembler, r Reg) *Assembler {
	lo, hi := lohi(r)
	if hi == 0 {
		asm.Bytes(0x31, 0xC0|lo|lo<<3)
	} else {
		asm.Bytes(0x48|hi<<1|hi<<2, 0x31, 0xC0|lo|lo<<3)
	}
	return asm
}

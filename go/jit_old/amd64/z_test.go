/*
 * onejit - JIT compiler in Go
 *
 * Copyright (C) 2018-2020 Massimiliano Ghilardi
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
 * z_test.go
 *
 *  Created on Feb 07, 2019
 *      Author Massimiliano Ghilardi
 */

package amd64

import (
	"testing"
)

func TestSoftRegId(t *testing.T) {
	var asm Asm
	asm.InitArch(Amd64{})

	var a, b, c SoftReg = MakeSoftReg(0, Uint64), MakeSoftReg(1, Uint64), MakeSoftReg(2, Uint64)
	code := []AsmCode{
		ALLOC, a,
		ALLOC, b,
		ALLOC, c,
		MOV, ConstUint64(1), a,
		MOV, ConstUint64(2), b,
		ADD3, a, b, c,
		FREE, a,
		FREE, b,
		FREE, c,
		RET,
	}
	asm.Assemble(code...)
	// t.Log(code)

	actual := asm.Code()
	expected := MachineCode{
		AMD64,
		[]uint8{
			0x48, 0xc7, 0xc3, 0x01, 0x00, 0x00, 0x00, //  movq	$1, %rbx
			0x48, 0xc7, 0xc6, 0x02, 0x00, 0x00, 0x00, //  movq	$2, %rsi
			0x48, 0x89, 0xdf, //                          movq	%rbx, %rdi
			0x48, 0x01, 0xf7, //                          addq	%rsi, %rdi
			0xc3, //                                      retq
		},
	}

	if !actual.Equal(expected) {
		t.Errorf("bad assembled code:\n\texpected %x\n\tactual   %x",
			expected, actual)
	}
}

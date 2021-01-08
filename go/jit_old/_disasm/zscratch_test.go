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
 * zscratch_test.go
 *
 *  Created on Jan 23, 2019
 *      Author Massimiliano Ghilardi
 */

package disasm

import (
	"testing"

	jit "github.com/cosmos72/onejit/go/jit_old"
	"github.com/cosmos72/onejit/go/jit_old/asm"
)

func parsehexdigit(ch byte) uint8 {
	switch {
	case ch >= '0' && ch <= '9':
		return ch - '0'
	case ch >= 'A' && ch <= 'F':
		return ch - 'A' + 10
	case ch >= 'a' && ch <= 'f':
		return ch - 'a' + 10
	default:
		return 0
	}
}

func HexToBinary(hex string) []byte {
	buf := make([]byte, len(hex)/2)
	for i := 0; i < len(hex)/2; i++ {
		buf[i] = parsehexdigit(hex[i*2])<<4 | parsehexdigit(hex[i*2+1])
	}
	return buf
}

func TestAmd64Shift2(t *testing.T) {
	code := asm.MachineCode{
		asm.AMD64,
		HexToBinary("488b742408488b761866c16e1805668b4e1866d36e18"),
	}
	PrintDisasm(t, code)

	_5 := asm.MakeConst(5, jit.Uint16)

	var c jit.Comp
	for _, archid := range [...]asm.ArchId{asm.AMD64, asm.ARM64} {
		c.InitArchId(archid)
		m := c.MakeVar(0, jit.Uint16)
		c.Compile(jit.Source{
			jit.SHR_ASSIGN, m, jit.NewExpr2(jit.SHR, m, _5),
		})
		t.Log(c.Code())

		PrintDisasm(t, c.Assemble())
	}
}
func TestAmd64Collatz(t *testing.T) {
	PrintDisasm(t, asm.MachineCode{
		asm.AMD64,
		HexToBinary("488b742408488b7618486b1e0348ffc34889df48d1ef48893e488b7c24084c8b4f3849ffc14c894f384c8b47404f8b04c848897c24184c89442410"),
	})
}

func TestAmd64Symbol(t *testing.T) {
	PrintDisasm(t, asm.MachineCode{
		asm.AMD64,
		HexToBinary("488b742408488b7618488b7c2408488b5f30488b5b18488b3b48897c2410"),
	})
}

func TestAmd64Defer3(t *testing.T) {
	PrintDisasm(t, asm.MachineCode{
		asm.AMD64,
		HexToBinary("488b742408488b7618c7462000000000c3" +
			"488b742408488b7618488b7c2408488b5f30488b5b30488b5b184c8b442408498b7830488b7f18488b3f48897b404c8b4424084d8b503849ffc24d8950384d8b48404f8b0cd14c894424184c894c2410c3"),
	})
}

func TestAmd64ForNested(t *testing.T) {
	PrintDisasm(t, asm.MachineCode{
		asm.AMD64,
		HexToBinary("488b742408488b7618488b7c2408488b5f30488b5b30488b5b30488b5b18488b3b48ffc748893bc3"),
	})
}

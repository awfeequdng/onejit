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
 * arch.go
 *
 *  Created on Feb 14, 2019
 *      Author Massimiliano Ghilardi
 */

package amd64

import (
	"fmt"

	"github.com/cosmos72/onejit/go/jit_old/common"
)

type Amd64 struct {
}

func init() {
	common.Archs[AMD64] = Amd64{}
}

// implement Arch interface
func (Amd64) Id() ArchId {
	return AMD64
}

func (Amd64) String() string {
	return NAME
}

func (Amd64) RegIdConfig() RegIdConfig {
	return RegIdConfig{
		RLo:  RLo,
		RHi:  RHi,
		RSP:  RSP,
		RVAR: RVAR,
		// allocate RAX, RDX, RCX as last because
		// amd64 shl, shr and div are hardcoded to use them
		RAllocFirst: RBX,
	}
}

func (Amd64) RegIdKind(id RegId) Kind {
	if id >= RAX && id <= R15 {
		return Uint64
	} else {
		return Invalid
	}
}

func (Amd64) RegIdValid(id RegId) bool {
	return id >= RLo && id <= RHi
}

func (Amd64) RegIdString(id RegId) string {
	var s string
	if id >= RLo && id <= RHi {
		s = regName8[id-RLo]
	}
	if s == "" {
		s = fmt.Sprintf("unknown_reg(%#x)", uint8(id))
	}
	return s
}

func (Amd64) RegValid(r Reg) bool {
	return r.RegId().Valid() && r.Kind().Size() != 0
}

func (Amd64) RegString(r Reg) string {
	id := r.RegId()
	if !id.Valid() {
		return fmt.Sprintf("%%unknown_reg(%#x,%v)", uint8(id), r.Kind())
	}
	id -= RLo
	var s string
	switch r.Kind().Size() {
	case 1:
		s = regName1[id]
	case 2:
		s = regName2[id]
	case 4:
		s = regName4[id]
	default:
		s = regName8[id]
	}
	return s
}

func (arch Amd64) MemString(m Mem) string {
	return fmt.Sprintf("%v@{%v+%v}", m.Kind(), m.RegId(), m.Offset())

}

func (Amd64) CodeString(code MachineCode) string {
	return fmt.Sprintf("%x", code.Bytes)
}

// Prologue used to add the following instruction to generated code,
// but now it does nothing, because adding ANY code is the user's responsibility:
//   movq 0x8(%rsp), %rdi
// equivalent to
//   asm.Asm(MOV, MakeMem(8, RSP, Uint64), MakeReg(RDI, Uint64))
func (Amd64) Prologue(asm *Asm) *Asm {
	return asm
}

func (Amd64) Epilogue(asm *Asm) *Asm {
	return asm.Op0(RET)
}

func (Amd64) Init(asm *Asm, start SaveSlot, end SaveSlot) *Asm {
	asm.RegIncUse(RSP) // stack pointer
	asm.RegIncUse(RBP) // frame pointer
	return asm
}

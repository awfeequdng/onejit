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
 *  Created on May 26, 2018
 *      Author Massimiliano Ghilardi
 */

package arm64

import (
	"fmt"

	"github.com/cosmos72/onejit/go/jit_old/common"
)

type Arm64 struct {
}

func init() {
	common.Archs[ARM64] = Arm64{}
}

// implement Arch interface
func (Arm64) Id() ArchId {
	return ARM64
}

func (Arm64) String() string {
	return NAME
}

func (Arm64) RegIdConfig() RegIdConfig {
	return RegIdConfig{
		RLo:         RLo,
		RHi:         RHi,
		RSP:         RSP,
		RVAR:        RVAR,
		RAllocFirst: RLo,
	}
}

func (Arm64) RegIdKind(id RegId) Kind {
	if id >= X0 && id <= XZR {
		return Uint64
	} else if id >= D0 && id <= D31 {
		return Float64
	}
	return Invalid
}

func (Arm64) RegIdValid(id RegId) bool {
	// XZR/XSP is valid only in few, hand-checked cases
	return id >= RLo && id <= RHi && id != XZR
}

func (Arm64) RegIdString(id RegId) string {
	var s string
	if id >= RLo && id <= RHi {
		s = regName64[id]
	}
	if s == "" {
		s = fmt.Sprintf("unknown_reg(%#x)", uint8(id))
	}
	return s
}

func (Arm64) RegValid(r Reg) bool {
	// XZR/XSP is valid only in few, hand-checked cases
	return r.RegId().Valid() && r.Kind().Size() != 0
}

func (Arm64) RegString(r Reg) string {
	var s string
	id := r.RegId()
	if id >= RLo && id <= RHi {
		switch r.Kind().Size() {
		case 1, 2, 4:
			s = regName32[id]
		case 8:
			s = regName64[id]
		}
	}
	if s == "" {
		s = fmt.Sprintf("unknown_reg(%#x,%v)", uint8(id), r.Kind())
	}
	return s
}

func (arch Arm64) MemString(m Mem) string {
	var regstr string
	regid := m.RegId()
	if regid == XZR {
		regstr = "xsp"
	} else {
		regstr = arch.RegIdString(regid)
	}
	return fmt.Sprintf("%v@{%s+%v}", m.Kind(), regstr, m.Offset())

}

// print arm64 machine code as sequence of 4-byte instructions
func (Arm64) CodeString(code MachineCode) string {
	const hexdigit string = "0123456789abcdef"
	bytes := code.Bytes
	i, j, n := 0, 0, len(bytes)
	buf := make([]byte, (n+3)/4*9)
	for ; i+4 <= n; i += 4 {
		buf[j+0] = hexdigit[bytes[i+3]>>4]
		buf[j+1] = hexdigit[bytes[i+3]&0xF]
		buf[j+2] = hexdigit[bytes[i+2]>>4]
		buf[j+3] = hexdigit[bytes[i+2]&0xF]
		buf[j+4] = hexdigit[bytes[i+1]>>4]
		buf[j+5] = hexdigit[bytes[i+1]&0xF]
		buf[j+6] = hexdigit[bytes[i+0]>>4]
		buf[j+7] = hexdigit[bytes[i+0]&0xF]
		buf[j+8] = ' '
		j += 9
	}
	for k := n - 1; k >= i; k-- {
		buf[j+0] = hexdigit[bytes[k]>>4]
		buf[j+1] = hexdigit[bytes[k]&0xF]
		j += 2
	}
	return string(buf[:j])
}

// Prologue used to add the following instruction to generated code,
// but now it does nothing, because adding ANY code is the user's responsibility:
//   ldr x29, [sp, #8]
// equivalent to:
// asm.Asm(MOV, MakeMem(8, XSP, Uint64), MakeReg(X29, Uint64))
func (Arm64) Prologue(asm *Asm) *Asm {
	// return asm.Uint32(0xf94007fd)
	// equivalent:
	// return asm.Asm(MOV, MakeMem(8, XSP, Uint64), MakeReg(X29, Uint64))
	return asm
}

func (arch Arm64) Epilogue(asm *Asm) *Asm {
	return arch.Op0(asm, RET)
}

func (Arm64) Init(asm *Asm, start SaveSlot, end SaveSlot) *Asm {
	asm.RegIncUse(X28) // pointer to goroutine-local data
	asm.RegIncUse(X30) // return address register
	asm.RegIncUse(XZR) // zero register / stack pointer
	return asm
}

/*
 * onejit - JIT compiler in Go
 *
 * Copyright (C) 2018-2019 Massimiliano Ghilardi
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
 * machine.go
 *
 *  Created on May 24, 2018
 *      Author Massimiliano Ghilardi
 */

package common

const (
	NoRegId RegId = 0
)

func (asm *Asm) RegIdConfig() RegIdConfig {
	return asm.arch.RegIdConfig()
}

func (asm *Asm) Op0(op Op0) *Asm {
	asm.arch.Op0(asm, op)
	return asm
}

func (asm *Asm) Op1(op Op1, dst Arg) *Asm {
	asm.arch.Op1(asm, op, dst)
	return asm
}

func (asm *Asm) Op2(op Op2, src Arg, dst Arg) *Asm {
	return asm.arch.Op2(asm, op, src, dst)
}

func (asm *Asm) Op3(op Op3, a Arg, b Arg, dst Arg) *Asm {
	return asm.arch.Op3(asm, op, a, b, dst)
}

func (asm *Asm) Op4(op Op4, a Arg, b Arg, c Arg, dst Arg) *Asm {
	return asm.arch.Op4(asm, op, a, b, c, dst)
}

func (asm *Asm) Zero(dst Arg) *Asm {
	return asm.arch.Op1(asm, ZERO, dst)
}

func (asm *Asm) Mov(src Arg, dst Arg) *Asm {
	return asm.arch.Op2(asm, MOV, src, dst)
}

func (asm *Asm) Load(src Mem, dst Reg) *Asm {
	return asm.arch.Op2(asm, MOV, src, dst)
}

func (asm *Asm) Store(src Reg, dst Mem) *Asm {
	return asm.arch.Op2(asm, MOV, src, dst)
}

func (asm *Asm) Cast(src Arg, dst Arg) *Asm {
	return asm.arch.Op2(asm, CAST, src, dst)
}

func (asm *Asm) Prologue() *Asm {
	return asm.arch.Prologue(asm)
}

func (asm *Asm) Epilogue() *Asm {
	return asm.arch.Epilogue(asm)
}

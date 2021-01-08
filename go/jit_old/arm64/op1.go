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
 * op1.go
 *
 *  Created on Jan 23, 2019
 *      Author Massimiliano Ghilardi
 */

package arm64

// ============================================================================
// one-arg instruction

func (arch Arm64) Op1(asm *Asm, op Op1, a Arg) *Asm {
	switch op {
	case ZERO:
		arch.zero(asm, a)
	case INC:
		arch.op3(asm, ADD3, a, MakeConst(1, a.Kind()), a)
	case DEC:
		arch.op3(asm, SUB3, a, MakeConst(1, a.Kind()), a)
	case NEG1, NOT1:
		arch.op2(asm, Op2(op), a, a)
	default:
		errorf("unknown Op1 instruction: %v %v", op, a)
	}
	return asm
}

// zero a register or memory location
func (arch Arm64) zero(asm *Asm, dst Arg) Arm64 {
	switch dst := dst.(type) {
	case Const:
		errorf("cannot zero a constant: %v %v", ZERO, dst)
	case Reg:
		arch.zeroReg(asm, dst)
	case Mem:
		arch.zeroMem(asm, dst)
	default:
		errorf("unknown destination type %T, expecting Reg or Mem: %v, %v", dst, ZERO, dst)
	}
	return arch
}

// zero a register
func (arch Arm64) zeroReg(asm *Asm, dst Reg) Arm64 {
	// alternative: return asm.movRegReg(MakeReg(XZR, dst.kind), dst)
	return arch.movConstReg(asm, MakeConst(0, dst.Kind()), dst)
}

// zero a memory location
func (arch Arm64) zeroMem(asm *Asm, dst Mem) Arm64 {
	arch.store(asm, MakeReg(XZR, dst.Kind()), dst)
	return arch
}

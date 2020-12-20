/*
 * gomacrojit - JIT compiler in Go
 *
 * Copyright (C) 2019 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * fmov.go
 *
 *  Created on Nov 15, 2019
 *      Author Massimiliano Ghilardi
 */

package arm64

import (
	"fmt"
)

func (arch Arm64) fload(asm *Asm, src Mem, dst Reg) Arm64 {
	return arch.floadstore(asm, 0, src, dst)
}

func (arch Arm64) fstore(asm *Asm, src Reg, dst Mem) Arm64 {
	return arch.floadstore(asm, 0, dst, src)
}

func (arch Arm64) fmov(asm *Asm, src Arg, dst Arg) Arm64 {
	assert(src.Kind().IsFloat() || dst.Kind().IsFloat())
	assert(SizeOf(src) == SizeOf(dst))

	if dst.Const() {
		errorf("destination cannot be a constant: %v %v, %v", MOV, src, dst)
	}
	if src == dst {
		return arch
	}

	switch dst := dst.(type) {
	case Reg:
		switch src := src.(type) {
		case Const:
			arch.fmovConstReg(asm, src, dst)
		case Reg:
			if src.RegId() != dst.RegId() {
				arch.fmovRegReg(asm, src, dst)
			}
		case Mem:
			arch.fload(asm, src, dst)
		default:
			errorf("unknown source type %T, expecting Const, Reg or Mem: %v %v, %v", src, MOV, src, dst)
		}
	case Mem:
		switch src := src.(type) {
		case Const:
			arch.fmovConstMem(asm, src, dst)
		case Reg:
			arch.fstore(asm, src, dst)
		case Mem:
			arch.fmovMemMem(asm, src, dst)
		default:
			errorf("unknown source type %T, expecting Const, Reg or Mem: %v %v, %v", src, MOV, src, dst)
		}
	default:
		errorf("unknown destination type %T, expecting Reg or Mem: %v %v, %v", dst, MOV, src, dst)
	}
	return arch
}

func (arch Arm64) fmovRegReg(asm *Asm, src Reg, dst Reg) Arm64 {
	srcIsFloat := src.Kind().IsFloat()
	dstIsFloat := dst.Kind().IsFloat()
	if srcIsFloat && dstIsFloat {
		asm.Uint32(0x1e204000 | fbit(dst) | fval(src)<<5 | fval(dst))
	} else if dstIsFloat {
		asm.Uint32(0x1e260000 | kbit(src) | fbit(dst) | kval(src)<<5 | fval(dst))
	} else if srcIsFloat {
		asm.Uint32(0x1e270000 | fbit(src) | kbit(dst) | fval(src)<<5 | kval(dst))
	} else {
		panic(fmt.Errorf("unimplemented Arm64.fmovRegReg(%v, %v)", src.Kind(), dst.Kind()))
	}
	return arch
}

func (arch Arm64) fmovConstReg(asm *Asm, c Const, dst Reg) Arm64 {
	cval := c.Val()
	xzr := MakeReg(XZR, dst.Kind())
	var immcval uint32
	var movk bool
	if cval >= 0 && cval < 0x10000 {
		immcval = 0x40<<19 | uint32(cval)
	} else if cval < 0 && cval >= -0x10000 {
		immcval = uint32(^cval)
	} else if arch.tryOp3RegConstReg(asm, OR3, xzr, uint64(cval), dst) {
		return arch
	} else if arch.tryOp3RegConstReg(asm, OR3, xzr, uint64(uint32(cval)), dst) {
		if dst.Kind().Size() == 8 {
			arch.movk(asm, uint16(cval>>32), 32, dst)
			arch.movk(asm, uint16(cval>>48), 48, dst)
		}
		return arch
	} else {
		immcval = 0x40<<19 | uint32(cval&0xFFFF)
		movk = true
	}
	asm.Uint32(kbit(dst) | 0x12800000 | immcval<<5 | kval(dst))
	if movk {
		arch.movk(asm, uint16(cval>>16), 16, dst)
		if dst.Kind().Size() == 8 {
			arch.movk(asm, uint16(cval>>32), 32, dst)
			arch.movk(asm, uint16(cval>>48), 48, dst)
		}
	}
	return arch
}

func (arch Arm64) fmovConstMem(asm *Asm, c Const, dst Mem) Arm64 {
	panic("unimplemented Arm64.fmovConstMem()")
}

func (arch Arm64) fmovMemMem(asm *Asm, src Mem, dst Mem) Arm64 {
	panic("unimplemented Arm64.fmovMemMem()")
}

func (arch Arm64) floadstore(asm *Asm, op loadstore, m Mem, r Reg) Arm64 {
	assert(SizeOf(m) == SizeOf(r))
	off := m.Offset()
	var sizebit uint32
	mrid := m.RegId()
	rid := r.RegId()

	switch m.Kind().Size() {
	case 4:
		sizebit = 0x8 << 28
		if off >= 0 && off <= 16380 && off%4 == 0 {
			asm.Uint32(sizebit | uint32(op) | uint32(off)<<8 | valOrX31(mrid, true)<<5 | valOrX31(rid, true))
			return arch
		}
	case 8:
		sizebit = 0xC << 28
		if off >= 0 && off <= 32760 && off%8 == 0 {
			asm.Uint32(sizebit | uint32(op) | uint32(off)<<7 | valOrX31(mrid, true)<<5 | valOrX31(rid, true))
			return arch
		}
	}
	// TO DO: load offset in a register?
	errorf("Mem offset [%v] too large for Arm64.floadstore()", off)
	return arch
}

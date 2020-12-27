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
 * aisgn.go
 *
 *  Created on Jan 23, 2019
 *      Author Massimiliano Ghilardi
 */

package amd64

import (
	. "github.com/cosmos72/gomacrojit/jit"
)

// %reg = const
func assignRegConst(asm *Assembler, r Reg, c Const) *Assembler {
	if c.IsZero() {
		return zeroReg(asm, r)
	}
	switch r.Size() {
	case 1:
		assignReg8Const(asm, r, c)
	case 2:
		assignReg16Const(asm, r, c)
	case 4:
		assignReg32Const(asm, r, c)
	case 8:
		assignReg64Const(asm, r, c)
	case 16:
		assignReg128Const(asm, r, c)
	}
	return asm
}

func assignReg8Const(asm *Assembler, r Reg, c Const) *Assembler {
	lo, hi := lohi(r)
	val := c.Int()
	if val == int64(int8(val)) {
		if r.RegId() < RSP {
			asm.Byte(0xB0 | lo)
		} else {
			asm.Bytes(0x40|hi, 0xB0|lo)
		}
		asm.Int8(int8(val))
	} else {
		Errorf("sign-extended constant overflows 8-bit destination: %v %v %v", r, ASSIGN, c)
	}
	return asm
}

func assignReg16Const(asm *Assembler, r Reg, c Const) *Assembler {
	lo, hi := lohi(r)
	val := c.Int()
	if val == int64(int16(val)) {
		if hi == 0 {
			asm.Bytes(0x66, 0xB8|lo)
		} else {
			asm.Bytes(0x66, 0x40|hi, 0xB8|lo)
		}
		asm.Int16(int16(val))
	} else {
		Errorf("sign-extended constant overflows 16-bit destination: %v %v %v", r, ASSIGN, c)
	}
	return asm
}

func assignReg32Const(asm *Assembler, r Reg, c Const) *Assembler {
	lo, hi := lohi(r)
	val := c.Int()
	if val == int64(int32(val)) {
		if hi == 0 {
			asm.Byte(0xB8 | lo)
		} else {
			asm.Bytes(0x40|hi, 0xB8|lo)
		}
		asm.Int32(int32(val))
	} else {
		Errorf("sign-extended constant overflows 16-bit destination: %v %v %v", r, ASSIGN, c)
	}
	return asm
}

func assignReg64Const(asm *Assembler, r Reg, c Const) *Assembler {
	lo, hi := lohi(r)
	val := c.Int()
	// 32-bit signed immediate constants, use mov
	if val == int64(int32(val)) {
		return asm.Bytes(0x48|hi, 0xC7, 0xC0|lo).Int32(int32(val))
	} else {
		// 64-bit constant, must use movabs
		return asm.Bytes(0x48|hi, 0xB8|lo).Int64(val)
	}
}

func assignReg128Const(asm *Assembler, r Reg, c Const) *Assembler {
	Errorf("unimplemented amd64.assignReg128Const()")
	return asm
}

// mem = const
func assignMemConst(asm *Assembler, m Amd64Mem, c Const) *Assembler {
	if m.IndexId() != NoRegId {
		Errorf("unimplemented: Amd64Mem with index register")
	}
	baseid := m.BaseId()
	kind := m.Kind()
	lo, hi := lohiId(baseid)
	offlen, offbit := offlen(m.Offset(), baseid)
	val := c.Int()
	switch kind.Size() {
	case 1:
		if hi == 0 {
			asm.Bytes(0xC6, offbit|lo)
		} else {
			asm.Bytes(0x40|hi, 0xC6, offbit|lo)
		}
	case 2:
		asm.Byte(0x66)
		fallthrough
	case 4:
		if hi == 0 {
			asm.Bytes(0xC7, offbit|lo)
		} else {
			asm.Bytes(0x40|hi, 0xC7, offbit|lo)
		}
	case 8:
		if val == int64(int32(val)) {
			asm.Bytes(0x48|hi, 0xC7, offbit|lo)
		} else {
			Errorf("constant too large, please copy it in a register: %v %v %v", ASSIGN, m, c)
			return asm
		}
	}
	quirk24(asm, baseid)
	switch offlen {
	case 1:
		asm.Int8(int8(m.Offset()))
	case 4:
		asm.Int32(m.Offset())
	}

	switch kind.Size() {
	case 1:
		asm.Int8(int8(val))
	case 2:
		asm.Int16(int16(val))
	case 4, 8:
		asm.Int32(int32(val))
	}
	return asm
}

/*
// ============================================================================
// movsx, movzx or mov
func cast(asm *Assembler, src Arg, dst Arg) *Assembler {
	if src == dst {
		return asm
	} else if SizeOf(src) == SizeOf(dst) {
		return asm.op2(asm, MOV, src, dst)
	}
	switch dst := dst.(type) {
	case Reg:
		switch src := src.(type) {
		case Reg:
			asm.castRegReg(asm, src, dst)
		case Mem:
			asm.castMemReg(asm, src, dst)
		case Const:
			src = src.Cast(dst.Kind())
			asm.movConstReg(asm, src, dst)
		default:
			Errorf("unsupported source type %T, expecting Reg, Mem or Const: %v %v %v", src, CAST, src, dst)
		}
	case Mem:
		switch src := src.(type) {
		case Reg:
			asm.castRegMem(asm, src, dst)
		case Mem:
			asm.castMemMem(asm, src, dst)
		case Const:
			src = src.Cast(dst.Kind())
			asm.op2ConstMem(asm, MOV, src, dst)
		default:
			Errorf("unsupported source type %T, expecting Reg, Mem or Const: %v %v %v", src, CAST, src, dst)
		}
	case Const:
		Errorf("destination cannot be a constant: %v %v %v", CAST, src, dst)
	default:
		Errorf("unsupported destination type %T, expecting Reg or Mem: %v %v %v", dst, CAST, src, dst)
	}
	return asm
}

func castRegReg(asm *Assembler, src Reg, dst Reg) *Assembler {
	var op uint8 = 0xB6 // movzx
	if dst.Kind().Signed() {
		op = 0xBE // movsx
	}
	lo, hi := lohi(dst)
	slo, shi := lohi(src)
	switch SizeOf(src) {
	case 1:
		// movzbq, movsbq
	case 2:
		op++ // movzwq, movswq
	case 4:
		if dst.Kind().Signed() {
			// movsd i.e. movslq
			asm.Bytes(0x48|hi<<2|shi, 0x63, 0xC0|lo<<3|slo)
			return asm
		}
		// amd64 does not have zero-extend 32bit -> 64bit
		// because operations that write into 32bit registers
		// already zero the upper 32 bits.
		// So just compile as a regular MOV
		fallthrough
	case 8:
		return asm.op2RegReg(asm, MOV, src, MakeReg(dst.RegId(), src.Kind()))
	default:
		Errorf("unsupported source register size %v, expecting 1, 2, 4 or 8: %v %v %v",
			SizeOf(src), CAST, src, dst)
	}
	// for simplicity, assume Sizeof(dst) == 8
	asm.Bytes(0x48|hi<<2|shi, 0x0F, op, 0xC0|lo<<3|slo)
	return asm
}

func castMemReg(asm *Assembler, src_m Mem, dst Reg) *Assembler {
	sregid := src_m.RegId()
	skind := src_m.Kind()

	var op uint8 = 0xB6 // movzx
	if skind.Signed() {
		op = 0xBE // movsx
	}
	lo, hi := lohi(dst)
	slo, shi := lohiId(sregid)
	offlen, offbit := offlen(src_m, sregid)
	// debugf("castMemReg() src = %v, dst = %v", src, dst)
	switch skind.Size() {
	case 1:
		// movzbq, movsbq
	case 2:
		op++ // movzwq, movswq
	case 4:
		if skind.Signed() {
			// sign-extend 32bit -> 64bit
			// movsd i.e. movslq
			asm.Bytes(0x48|hi<<2|shi, 0x63, offbit|lo<<3|slo)
			quirk24(asm, sregid)
			switch offlen {
			case 1:
				asm.Int8(int8(src_m.Offset()))
			case 4:
				asm.Int32(src_m.Offset())
			}
			return asm
		}
		// amd64 does not have zero-extend 32bit -> 64bit
		// because operations that write into 32bit registers
		// already zero the upper 32 bits.
		// So just compile as a regular MOV
		// debugf("zero-extend 32bit -> 64bit: dst = %v, src = %v", dst, m)
		fallthrough
	case 8:
		asm.op2MemReg(asm, MOV, src_m, MakeReg(dst.RegId(), skind))
		return asm
	default:
		Errorf("invalid source register size %v, expecting 1, 2, 4 or 8: %v %v %v",
			skind.Size(), CAST, src_m, dst)
	}
	// for simplicity, assume Sizeof(dst) == 8
	asm.Bytes(0x48|hi<<2|shi, 0x0F, op, offbit|lo<<3|slo)
	quirk24(asm, sregid)
	switch offlen {
	case 1:
		asm.Int8(int8(src_m.Offset()))
	case 4:
		asm.Int32(src_m.Offset())
	}
	return asm
}

func castRegMem(asm *Assembler, src Reg, dst_m Mem) *Assembler {
	dkind := dst_m.Kind()
	// assume that user code cannot use the same register
	// multiple times with different kinds
	r := MakeReg(src.RegId(), dkind)
	asm.castRegReg(asm, src, r)
	return asm.op2RegMem(asm, MOV, r, dst_m)
}

func castMemMem(asm *Assembler, src Mem, dst Mem) *Assembler {
	if SizeOf(src) > SizeOf(dst) && !src.Kind().IsFloat() {
		// just read the lowest bytes from src
		asm.op2MemMem(asm, MOV,
			MakeMem(src.Offset(), src.RegId(), dst.Kind()),
			dst)
	} else {
		r := asm.RegAlloc(dst.Kind())
		asm.castMemReg(asm, src, r)
		asm.op2RegMem(asm, MOV, r, dst)
		asm.RegFree(r)
	}
	return asm
}
*/
